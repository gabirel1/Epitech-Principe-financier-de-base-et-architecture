#include <cstring>

#include "Common/Core/Logger.hpp"
#include "Client/Processor/OrderBook.hpp"
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

namespace proc
{
    std::optional<fix::Message> OrderBook::process(const std::string &_entry, Context &_ctx)
    {
        std::ignore = _ctx;

        std::vector<std::string> words = utils::space_split(_entry);
        std::vector<const char *> cwords;
        OrderType side;
        std::string symbol;
        int param = 0;

        if (words.empty())
            return {};
        if (words.at(0) == "ob") {
            for (auto &_word : words)
                cwords.emplace_back(_word.c_str());
            while (param != -1) {
                param = getopt(cwords.size(), const_cast<char * const *>(cwords.data()), "s:S:");
                switch (param) {
                    case 'S': symbol = optarg;
                        break;
                    case 's': {
                        if (std::strcmp(optarg, "ask") == 0)
                            side = OrderType::Ask;
                        else if (std::strcmp(optarg, "bid") == 0)
                            side = OrderType::Bid;
                        else
                            return {};
                        break;
                    }
                    case -1:
                        break;
                    default:
                        return {};
                };
            }
            if (side == OrderType::Ask && m_ask.contains(symbol)) {
                std::cout << "Ask table of " << symbol << std::endl;
                displayBook<AskBook>(m_ask.at(symbol));
            } else if (side == OrderType::Bid && m_bid.contains(symbol)) {
                std::cout << "Bid table of " << symbol << std::endl;
                displayBook<BidBook>(m_bid.at(symbol));
            }
        }
        return {};
    }

    std::optional<fix::Message> OrderBook::process(fix::Serializer::AnonMessage &_msg, Context &_context)
    {
        if (!_context.Loggin && _msg.at(fix::Tag::MsgType) != fix::Logon::MsgType) {
            Logger::Log("[TCPOutput] You are not logged in, please log in first");
            return {};
        }
        if (_msg.at(fix::Tag::MsgType) != fix::MarketDataIncrementalRefresh::MsgType &&
            _msg.at(fix::Tag::MsgType) != fix::MarketDataSnapshotFullRefresh::MsgType)
            return {};
        else if (_msg.at(fix::Tag::MsgType) == fix::MarketDataSnapshotFullRefresh::MsgType)
            treatFullRefresh(_msg);
        else if (_msg.at(fix::Tag::MsgType) == fix::MarketDataIncrementalRefresh::MsgType)
            treatIncrRefresh(_msg);
        return {};
    }

    std::optional<data::UDPPackage> OrderBook::process(const data::UDPPackage &_package, Context &_context)
    {
        std::ignore = _context;

        Logger::Log("[UDPOutput] receive new actions: ", _package); // todo log
        if (UDP_FLAG_GET_BOOK(_package.flag) == OrderType::Bid)
            functional_sync(m_bid, _package.symbol, _package.price, _package.quantity, OrderBook::QtySync);
        else
            functional_sync(m_ask, _package.symbol, _package.price, _package.quantity, OrderBook::QtySync);
        return {};
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
    }

    data::IncrRefresh OrderBook::loadIncrRefresh(fix::Serializer::AnonMessage &_msg)
    {
        size_t size = utils::to<size_t>(_msg.at(fix::Tag::NoMDEntries));
        data::IncrRefresh refresh{size};

        refresh.size = size;
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
        size_t size = utils::to<size_t>(_msg.at(fix::Tag::NoMDEntryTypes));
        data::FullRefresh refresh;

        refresh.size = size;
        refresh.symbol = _msg.at(fix::Tag::Symbol);
        Logger::Log("[FIX] {LoadFullRefresh} Loading: ", size, " modifications on symbol: ", refresh.symbol);
        std::vector<Quantity> quantitys = data::extract<Quantity>(_msg, fix::Tag::MinQty);
        std::vector<Price> prices = data::extract<Price>(_msg, fix::Tag::MDEntryPx);
        std::vector<int> types = data::extract<int>(_msg, fix::Tag::MDEntryType);
        for (size_t it = 0; it < size; it++) {
            refresh.quantitys.push_back(quantitys[it]);
            refresh.prices.push_back(prices[it]);
            refresh.types.push_back(static_cast<OrderType>(types[it]));
            Logger::Log("[FIX] {LoadFullRefresh} Symbol: ", refresh.symbol ,", Quantity: ", quantitys[it], ", prices: ", prices[it], ", type: ", types[it]);
        }
        return refresh;
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
}