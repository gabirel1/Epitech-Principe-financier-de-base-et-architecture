#include "Common/Core/Logger.hpp"
#include "Client/Data/OderBook.hpp"
#include "Common/Message/MarketDataIncrementalRefresh.hpp"
#include "Common/Message/MarketDataRequest.hpp"
#include "Common/Message/MarketDataSnapshotFullRefresh.hpp"
#include "Common/Message/Tag.hpp"

namespace data
{
    IncrRefresh::IncrRefresh(size_t _size)
        : size(_size), prices(size), types(size), quantitys(size), symbols(size)
    {
    }
}

OrderBook::OrderBook(const std::string &_name, UDPOutput &_udp, TCPOutput &_tcp, TCPInput &_output)
    : m_name(_name), m_udp(_udp), m_tcp(_tcp), m_output(_output)
{
}

bool OrderBook::start()
{
    if (!m_running) {
        m_thread = std::thread([this] () {
            loop();
        });
        m_running = true;
    }
    return m_running;
}

bool OrderBook::stop()
{
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
        return true;
    }
    return false;
}

std::shared_ptr<OrderBook::BidBook> OrderBook::getBid(const std::string &_sym)
{
    return m_bid[_sym];
}

std::shared_ptr<OrderBook::AskBook> OrderBook::getAsk(const std::string &_sym)
{
    return m_ask[_sym];
}

void OrderBook::loop()
{
    sendFullRefresh();
    sendSubscribe();

    while (m_running) {
        if (!m_udp.empty()) {
            const data::UDPPackage package = m_udp.front();

            Logger::Log("[UDPOutput] receive new actions: ", package); // todo log
            if (UDP_FLAG_GET_BOOK(package.flag) == OrderType::Bid)
                functional_sync(m_bid, package.symbol, package.price, package.quantity, OrderBook::QtySync);
            else
                functional_sync(m_ask, package.symbol, package.price, package.quantity, OrderBook::QtySync);
            m_tcp.pop();
        }
        if (!m_tcp.empty()) {
            fix::Serializer::AnonMessage &msg = m_tcp.front();

            Logger::Log("[TCPOutput] received new message: { MsgType: ", msg.at(fix::Tag::MsgType), " }");
            if (msg.at(fix::Tag::MsgType) != "" && msg.at(fix::Tag::MsgType) != "") {
                Logger::Log("[TCPOutput] Message not handle by OrderBook");
                continue;
            }
            if (msg.at(fix::Tag::MsgType) == "")
                treatFullRefresh(msg);
            else if (msg.at(fix::Tag::MsgType) == "")
                treatIncrRefresh(msg);
            m_tcp.pop();
        }
    }
}

void OrderBook::treatFullRefresh(fix::Serializer::AnonMessage &_msg)
{
    data::FullRefresh refresh = loadFullRefresh(_msg);
    BidMap bid;
    AskMap ask;

    Logger::Log("[TCPOutput] {FullRefresh} Refreshing: ", refresh.size, " elements");
    for (size_t it = 0; it < refresh.size; it++) {
        if (refresh.types[it] == OrderType::Bid)
            functional_sync(bid, refresh.symbol, refresh.prices[it], refresh.quantitys[it], OrderBook::CopySync);
        else
            functional_sync(ask, refresh.symbol, refresh.prices[it], refresh.quantitys[it], OrderBook::CopySync);
    }

    Logger::Log("[TCPOutput] {FullRefresh} Synchronisation of last book: Bid");
    sync_book(bid, m_bid_last, OrderBook::CopySync);
    Logger::Log("[TCPOutput] {FullRefresh} Synchronisation of last book: Ask");
    sync_book(ask, m_ask_last, OrderBook::CopySync);
    Logger::Log("[TCPOutput] {FullRefresh} Synchronisation of actual book: Bid");
    sync_book(bid, m_bid, OrderBook::CopySync);
    Logger::Log("[TCPOutput] {FullRefresh} Synchronisation of actual book: Ask");
    sync_book(ask, m_ask, OrderBook::CopySync);
}

void OrderBook::treatIncrRefresh(fix::Serializer::AnonMessage &_msg)
{
    if (m_is_sync) {
        data::IncrRefresh refresh = loadIncrRefresh(_msg);
        BidMap bid;
        AskMap ask;

        for (size_t it = 0; it < refresh.size; it++) {
            if (refresh.types[it] == OrderType::Bid)
                functional_sync(bid, refresh.symbols[it], refresh.prices[it], refresh.quantitys[it], OrderBook::QtySync);
            else
                functional_sync(ask, refresh.symbols[it], refresh.prices[it], refresh.quantitys[it], OrderBook::QtySync);
        }

        Logger::Log("[TCPOutput] {IncrRefresh} Synchronisation of last book: Bid");
        sync_book(bid, m_bid_last, OrderBook::QtySync);
        Logger::Log("[TCPOutput] {IncrRefresh} Synchronisation of last book: Ask");
        sync_book(ask, m_ask_last, OrderBook::QtySync);
        Logger::Log("[TCPOutput] {IncrRefresh} Synchronisation of actual book: Bid");
        sync_book(m_bid_last, m_bid, OrderBook::CopySync);
        Logger::Log("[TCPOutput] {IncrRefresh} Synchronisation of actual book: Ask");
        sync_book(m_ask_last, m_ask, OrderBook::CopySync);
    } else {
        Logger::Log("[TCPOutput] {IncrRefresh} Initialisation of final synchronisation");
        sendFullRefresh();
        m_is_sync = true;
    }
}

data::IncrRefresh OrderBook::loadIncrRefresh(fix::Serializer::AnonMessage &_msg)
{
    size_t size = utils::to<size_t>(_msg.at(fix::Tag::NoMDEntries));
    data::IncrRefresh refresh{size};

    Logger::Log("[FIX] {LoadIncrRefresh} Loading: ", size, " modifications");
    std::vector<Quantity> quantitys = data::extract<Quantity>(_msg, fix::Tag::MinQty);
    std::vector<Price> prices = data::extract<Price>(_msg, fix::Tag::MDEntryPx);
    std::vector<OrderType> types = data::extract<OrderType>(_msg, fix::Tag::MDEntryType);
    std::vector<std::string> symbols = data::extract<std::string>(_msg, fix::Tag::Symbol);
    for (size_t it = 0; it < size; it++) {
        refresh.quantitys.push_back(quantitys[it]);
        refresh.prices.push_back(prices[it]);
        refresh.types.push_back(types[it]);
        refresh.symbols.push_back(symbols[it]);
    }
    return refresh;
}

data::FullRefresh OrderBook::loadFullRefresh(fix::Serializer::AnonMessage &_msg)
{
    size_t size = utils::to<size_t>(_msg.at(fix::Tag::NoMDEntries));
    data::FullRefresh refresh{size};

    refresh.symbol = _msg.at(fix::Tag::Symbol);
    Logger::Log("[FIX] {LoadIncrRefresh} Loading: ", size, " modifications on symbol: ", refresh.symbol);
    std::vector<Quantity> quantitys = data::extract<Quantity>(_msg, fix::Tag::MinQty);
    std::vector<Price> prices = data::extract<Price>(_msg, fix::Tag::MDEntryPx);
    std::vector<OrderType> types = data::extract<OrderType>(_msg, fix::Tag::MDEntryType);
    for (size_t it = 0; it < size; it++) {
        refresh.quantitys.push_back(quantitys[it]);
        refresh.prices.push_back(prices[it]);
        refresh.types.push_back(types[it]);
    }
    return refresh;
}

void OrderBook::sendFullRefresh()
{
    fix::MarketDataRequest request;
    const std::vector<std::string> symbols{ MARKET_NAME };
    std::string lsymbols;

    request.set146_noRelatedSym(std::to_string(symbols.size()));
    for (const auto &_sym : symbols)
        lsymbols += _sym + ",";
    if (!symbols.empty())
        lsymbols.erase(lsymbols.end());
    request.set55_symbol(lsymbols);
    request.set262_mDReqID(utils::id());
    request.set263_subscriptionRequestType("0");
    request.set264_marketDepth("0");
    request.set267_noMDEntryTypes("2");
    request.set269_mDEntryType("0,1");
    Logger::Log("[TCPInput] Sending a data full refresh request");
    m_output.push(std::move(request));
}

void OrderBook::sendSubscribe()
{
    fix::MarketDataRequest request;
    const std::vector<std::string> symbols{ MARKET_NAME };
    std::string lsymbols;

    request.set146_noRelatedSym(std::to_string(symbols.size()));
    for (const auto &_sym : symbols)
        lsymbols += _sym + ",";
    if (!symbols.empty())
        lsymbols.erase(lsymbols.end());
    request.set55_symbol(lsymbols);
    request.set262_mDReqID(utils::id());
    request.set263_subscriptionRequestType("1");
    request.set264_marketDepth("0");
    request.set267_noMDEntryTypes("2");
    request.set269_mDEntryType("0,1");
    Logger::Log("[TCPInput] Sending a data incremental refresh request");
    m_output.push(std::move(request));
}

Quantity OrderBook::QtySync(Quantity _left, Quantity _right)
{
    return _left + _right;
}

Quantity OrderBook::CopySync(Quantity _left, Quantity _right)
{
    std::ignore = _right;

    return _left;
}