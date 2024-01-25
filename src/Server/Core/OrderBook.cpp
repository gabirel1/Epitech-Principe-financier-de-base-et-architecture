#include <numeric>

#include "Server/Core/OrderBook.hpp"
#include "Common/Message/ExecutionReport.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

OrderBook::OrderBook(const std::string &_name, EventQueue &_output)
    : m_name(_name), m_output(_output)
{
}

bool OrderBook::add(OrderType _type, Price _price, Order& _order)
{
    if (has(_type, _order.orderId))
        return false;
    add(_type, _price, _order, OrderStatus::New);
    return true;
}

bool OrderBook::modify(OrderType _type, Price _price, Order &_order)
{
    if (has(_type, _order.orderId))
        return false;
    add(_type, _price, _order, OrderStatus::Replaced);
    return true;
}

bool OrderBook::cancel(OrderType _type, OrderId _orderId, bool _notif)
{
    if (_type == OrderType::Ask)
        return cancel<AskBook>(m_ask_id, _orderId, _notif);
    return cancel<BidBook>(m_bid_id, _orderId, _notif);
}

bool OrderBook::has(OrderType _type, OrderId _orderId) const
{
    if (_type == OrderType::Ask)
        return m_ask_id.contains(_orderId);
    return m_bid_id.contains(_orderId);
}

std::vector<Price> OrderBook::getPrice(OrderType _type)
{
    if (_type == OrderType::Ask)
        return inter_getPrice<AskBook>(m_ask);
    return inter_getPrice<BidBook>(m_bid);
}

bool OrderBook::contain(OrderType _type, Price _price)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    if (_type == OrderType::Ask)
        return m_ask.contains(_price);
    return m_bid.contains(_price);
}

const OrderList& OrderBook::getOrders(OrderType _type, Price _price)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    if (_type == OrderType::Ask)
        return m_ask.at(_price);
    return m_bid.at(_price);
}


Quantity OrderBook::sumQuantity(OrderType _type, Price _price)
{
    const OrderList &ref = getOrders(_type, _price);
    std::lock_guard<std::mutex> guard(m_mutex);

    return std::accumulate(ref.begin(), ref.end(), 0ull, [] (Quantity _total, const Order& _order) {
        return std::move(_total) + _order.quantity;
    });
}

void OrderBook::add(OrderType _type, Price _price, Order &_order, OrderStatus _status)
{
    Event event;

    event.orderId = _order.orderId;
    event.orgQty = _order.quantity;
    event.userId = _order.userId;
    event.price = _price;
    event.side = _type;
    std::cout << "\n\n\n\n\nEVENT.SIDE == " << ((event.side == OrderType::Ask) ? "ASK (4)" : "BID (3)") << "\n"; 
    std::cout << "_TYPE == " << ((_type == OrderType::Ask) ? "ASK (4)" : "BID (3)") << "\n";
    std::cout << "_price == " << _price << "\n\n\n\n\n";
    event.sold = false;
    if (_type == OrderType::Bid) {
        if (add<AskBook, std::greater_equal<Price>>(m_ask, _price, _order)) {
            std::lock_guard<std::mutex> guard(m_mutex);

            Logger::Log("[OrderBook] (", m_name, ") {Add} New order in BID: ", _order, " at price: ", _price);
            m_bid[_price].push_back(_order);
            m_bid_id.emplace(_order.orderId, std::make_pair(m_bid.find(_price), m_bid.at(_price).end() - 1));
            event.status = OrderStatus::PartiallyFilled;
        }
    }
    else {
        if (add<BidBook, std::less_equal<Price>>(m_bid, _price, _order)) {
            std::lock_guard<std::mutex> guard(m_mutex);

            Logger::Log("[OrderBook] (", m_name, ") {Add} New order in ASK: ", _order, " at price: ", _price);
            m_ask[_price].push_back(_order);
            m_ask_id.emplace(_order.orderId, std::make_pair(m_ask.find(_price), m_ask.at(_price).end() - 1));
        }
    }
    event.quantity = _order.quantity;
    if (event.quantity == event.orgQty)
        event.status = _status;
    else if (event.quantity != 0)
        event.status = OrderStatus::PartiallyFilled;
    else
        event.status = OrderStatus::Filled;
    Logger::Log("[OrderBook] (", m_name, ") {Add} New order event: "); // todo log
    m_output.append(event);
    Logger::Log("[OrderBook] (", m_name, ") {Add} New order event send");
}