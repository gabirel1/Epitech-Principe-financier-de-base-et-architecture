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
    std::optional<fix::Message> OrderBook::process(fix::Serializer::AnonMessage &_msg, Context &_context)
    {
        if (!_context.Loggin && _msg.at(fix::Tag::MsgType) != fix::Logon::MsgType) {
            Logger::Log("[TCPOutput] You are not logged in, please log in first");
            return {};
        }
        Logger::Log("[TCPOutput] received new message: { MsgType: ", _msg.at(fix::Tag::MsgType), " }");
        if (_msg.at(fix::Tag::MsgType) == fix::Reject::MsgType)
            Logger::Log("[TCPOutput] There was an error with your request: ", _msg.at(fix::Tag::Text), ", please try again (make sure you are logged in)");
        else if (_msg.at(fix::Tag::MsgType) != fix::MarketDataIncrementalRefresh::MsgType &&
            _msg.at(fix::Tag::MsgType) != fix::MarketDataSnapshotFullRefresh::MsgType && _msg.at(fix::Tag::MsgType) != fix::ExecutionReport::MsgType)
            Logger::Log("[TCPOutput] Message not handle by OrderBook");
        else if (_msg.at(fix::Tag::MsgType) == fix::MarketDataSnapshotFullRefresh::MsgType)
            treatFullRefresh(_msg);
        else if (_msg.at(fix::Tag::MsgType) == fix::MarketDataIncrementalRefresh::MsgType)
            treatIncrRefresh(_msg);
        else if (_msg.at(fix::Tag::MsgType) == fix::ExecutionReport::MsgType)
            treatExecutionReport(_msg, _context);
        return {};
    }

    std::optional<fix::Message> OrderBook::build(char _tag, Context &_context) const
    {
        std::ignore = _context;

        if (_tag == fix::MarketDataSnapshotFullRefresh::cMsgType)
            return buildFullRefresh();
        else if (_tag == fix::MarketDataSnapshotFullRefresh::cMsgType)
            return buildIncrRefresh();
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

    void OrderBook::treatExecutionReport(fix::Serializer::AnonMessage &_msg, Context &_ctx)
    {
        Logger::Log("[TCPOutput] {Execution Report} New execution report received");
        std::string orderId = _msg.at(fix::Tag::OrderID);
        std::string quantity = _msg.at(fix::Tag::OrderQty);
        std::string price = _msg.at(fix::Tag::Price);
        std::string side = _msg.at(fix::Tag::Side);
        std::string symbol = _msg.at(fix::Tag::Symbol);

        Logger::Log("[TCPOutput] {NewOrder} New order: ", orderId, " ", quantity, " ", price, " ", side, " ", symbol);
        // if (side == "1")
        //     functional_sync(m_bid, symbol, price, quantity, OrderBook::QtySync);
        // else
        //     functional_sync(m_ask, symbol, price, quantity, OrderBook::QtySync);
        OrderClient _order;

        _order.orderId = orderId;
        _order.quantity = std::stoi(quantity);
        _order.price = std::stoi(price);
        _order.type = (side == "1" ? OrderType::Bid : OrderType::Ask);
        _order.symbol = symbol;

        std::cout << "[1]_ctx.MyOrders.size() = " << _ctx.MyOrders.size() << std::endl;
        _ctx.MyOrders.push_back(_order);
        std::cout << "[2]_ctx.MyOrders.size() = " << _ctx.MyOrders.size() << std::endl;
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
        data::FullRefresh refresh;

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

    fix::Message OrderBook::buildFullRefresh() const
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
        return request;
    }

    fix::Message OrderBook::buildIncrRefresh() const
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
        return request;
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