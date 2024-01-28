#include "Client/Data/OderBook.hpp"
#include "Common/Message/MarketDataIncrementalRefresh.hpp"
#include "Common/Message/MarketDataRequest.hpp"
#include "Common/Message/MarketDataSnapshotFullRefresh.hpp"
#include "Common/Message/Tag.hpp"

namespace data
{
    IncrRefresh::IncrRefresh(size_t _size)
        : m_size(_size), prices(m_size), types(m_size), quantitys(m_size), symbols(m_size)
    {
    }
}

OrderBook::OrderBook(const std::string &_name, UDPInput &_udp, TCPInput &_tcp, TCPInput &_output)
    : m_name(_name), m_udp(_udp), m_tcp(_tcp), m_output(_output)
{
}

bool OrderBook::start()
{
    if (!m_running) {
        m_thread = std::thread(&loop, this);
        m_running = true;
    }
    return m_running;
}

bool OrderBook::stop()
{
    m_running = false;
    if (m_thread.is_joinable())
        return m_thread.join();
    return false;
}

std::shared_ptr<BidBook> &OrderBook::getBid(const std::string &_sym)
{
    return m_bid[_sym];
}

std::shared_ptr<AskBook> &OrderBook::getAsk(const std::string &_sym)
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
            // calculate udp application on m_bid/m_ask
            // need symbol
        }
        if (!m_tcp.empty()) {
            fix::Serializer::AnonMessage &msg = m_tcp.front();

            if (msg.at(fix::Tag::MsgType) != "" && msg.at(fix::Tag::MsgType) != "")
                continue;
            if (msg.at(fix::Tag::MsgType) == "")
                treatFullRefresh(msg)
            else if (msg.at(fix::Tag::MsgType) == "")
                treatIncrRefresh(msg)
            if (m_tcp.front() == msg)
                m_tcp.pop();
        }
    }
}

void OrderBook::treatFullRefresh(fix::Serializer::AnonMessage &_msg)
{
    data::FullRefresh refresh = loadFullRefresh(_msg);
    BidMap bid;
    AskMap ask;

    for (size_t it = 0; it < refresh.size; it++) {
        if (refresh.types.at(it) == OrderType::Bid)
            bid[refresh.symbol][refresg.prices] = refresh.quantitys[it];
        if (refresh.types.at(it) == OrderType::Bid)
            ask[refresh.symbol][refresg.prices] = refresh.quantitys[it];
    }

    sync_book(bid, m_bid_last, OrderBook::QtyOverwrite);
    sync_book(ask, m_ask_last, OrderBook::QtyOverwrite);
    sync_book(bid, m_bid, OrderBook::QtyOverwrite);
    sync_book(ask, m_ask, OrderBook::QtyOverwrite);
}

void OrderBook::treatIncrRefresh(fix::Serializer::AnonMessage &_msg)
{
    if (m_is_sync) {
        data::IncrRefresh refresh = loadIncrRefresh(_msg);
        BidMap bid;
        AskMap ask;

        for (size_t it = 0; it < refresh.size; it++) {
            if (refresh.types.at(it) == OrderType::Bid)
                bid[refresh.symbols[it]][refresh.prices[it]] = refresh.quantitys[it];
            else
                ask[refresh.symbols[it]][refresh.prices[it]] = refresh.quantitys[it];
        }

        sync_book(bid, m_bid_last, OrderBook::QtySync);
        sync_book(ask, m_ask_last, OrderBook::QtySync);
        sync_book(m_bid_last, m_bid, OrderBook::CopySync);
        sync_book(m_ask_last, m_ask, OrderBook::CopySync);
    } else {
        sendFullRefresh();
        m_is_sync = true;
    }
}

data::IncrRefresh OrderBook::loadIncrRefresh(fix::Serializer::AnonMessage &_msg)
{
    size_t size = utils::to<size_t>(_msg.at(fix::Tag::NoMDEntries));
    data::IncrRefresh refresh{size};

    std::vector<Quantity> quantitys = extract<Quantity>(_msg, fix::Tag::MinQty);
    std::vector<Price> prices = extract<Price>(_msg, fix::Tag::MDEntryPx);
    std::vector<Price> types = extract<Price>(_msg, fix::Tag::MDEntryType);
    std::vector<Price> symbols = extract<Price>(_msg, fix::Tag::Symbol);
    for (size_t it = 0; it < size; it++) {
        refresh.quantitys.push_back(quantitys[it]);
        refresh.prices.push_back(prices[it]);
        refresh.types.push_back(types[it]);
        refresh.symbols.push_back(symbol[it]);
    }
    return refresh;
}

data::FullRefresh OrderBook::loadFullRefresh(fix::Serializer::AnonMessage &_msg)
{
    size_t size = utils::to<size_t>(_msg.at(fix::Tag::NoMDEntries));
    data::FullRefresh refresh{size};

    std::vector<Quantity> quantitys = extract<Quantity>(_msg, fix::Tag::MinQty);
    std::vector<Price> prices = extract<Price>(_msg, fix::Tag::MDEntryPx);
    std::vector<Price> types = extract<Price>(_msg, fix::Tag::MDEntryType);
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
    const std::vector<std::string> symbols = { MARKET_DATA_SYM }
    std::string lsymbols;

    request.set146_noRelatedSym(std::to_string(symbols.size()));
    for (const auto &_sym : symbols)
        lsymbols += _sym ",";
    if (!symbols.empty())
        lsymbols.erase(lsymbols.end());
    request.set55_symbol(lsymbol);
    request.set262_mDReqID(); // generate randome id
    request.set263_subscriptionRequestType("0");
    request.set264_marketDepth("0");
    request.set267_noMDEntryTypes("2");
    request.set269_mDEntryType("0,1");
    // send over TCP
}

void OderBook::sendSubscribe()
{
    fix::MarketDataRequest request;
    const std::vector<std::string> symbols = { MARKET_DATA_SYM }
    std::string lsymbols;

    request.set146_noRelatedSym(std::to_string(symbols.size()));
    for (const auto &_sym : symbols)
        lsymbols += _sym ",";
    if (!symbols.empty())
        lsymbols.erase(lsymbols.end());
    request.set55_symbol(lsymbol);
    request.set262_mDReqID(); // generate randome id
    request.set263_subscriptionRequestType("2");
    request.set264_marketDepth("0");
    request.set267_noMDEntryTypes("2");
    request.set269_mDEntryType("0,1");
    // send over TCP
}

Quantity OrderBook::QtySync(Quantity _left, Quantity _right)
{
    return _left + _right;
}

Quantity OrderBook::CopySync(Quantity _left, Quantity _right)
{
    return _left;
}

template<class T, class T>
void OrderBook::sync_book(T &_origine, _T &_target, SyncFn _sync)
{
    for (auto [_sym, _book] : _origine) {
        for (auto [_price, _qty] : _book) {
            _target[_symbol][_price] = _sync(_target[_symbol][_price], _qty);
            if (_target[_symbol][_price] <= 0)
                _target[_symbol].erase(_price);
        }
    }
}