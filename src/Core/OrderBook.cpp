#include <numeric>

#include "Core/OrderBook.hpp"

bool OrderBook::add(OrderType _type, Price _price, Order& _order)
{
    bool res = false;

    if (_type == OrderType::Bid) {
        if (add<AskBook>(m_ask, _price, _order)) {
            std::lock_guard<std::mutex> guard(m_mutex);

            m_bid.at(_price).push_back(_order);
        }
    }
    else {
        if (add<BidBook>(m_bid, _price, _order)) {
            std::lock_guard<std::mutex> guard(m_mutex);
            m_ask.at(_price).push_back(_order);
        }
    }
    return res;
}

void OrderBook::modify(OrderType _type, Price _price, Price _oprice, Order& _order)
{
    if (_type == OrderType::Ask)
        modify<BidBook>(m_bid, _price, _oprice, _order);
    else
        modify<AskBook>(m_ask, _price, _oprice, _order);
}

bool OrderBook::cancel(OrderType _type, Price _price, UserId _userId, OrderId _orderId)
{
    if (_type == OrderType::Ask)
        return cancel<AskBook>(m_ask, _price, _userId, _orderId);
    return cancel<BidBook>(m_bid, _price, _userId, _orderId);
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