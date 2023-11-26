#include <numeric>

#include "OrderBook.hpp"

std::vector<double> OrderBook::getPrice(OrderType _type) const
{
    if (_type == OrderType::Ask)
        return getPrice<AskBook>(m_ask);
    return getPrice<BidBook>(m_bid);
}

bool OrderBook::contain(OrderType _type, Price _price) const
{
    if (_type == OrderType::Ask)
        return m_ask.contains(_price);
    return m_bid.contains(_price);
}

const OrderList &OrderBook::getOrders(OrderType _type, Price _price) const
{
    if (_type == OrderType::Ask)
        return m_ask.at(_price);
    return m_bid.at(_price);
}

Quantity OrderBook::sumQuantity(OrderType _type, Price _price) const
{
    const OrderList &ref = getOrders(_type, _price);

    return std::accumulate(ref.begin(), ref.end(), 0ull, [] (Quantity _total, const Order& _order) {
        return std::move(_total) + _order.quantity;
    });
}

bool OrderBook::add(OrderType _type, Price _price, Order &_order)
{
    bool res = false;

    if (_type == OrderType::Bid) {
        if (add<AskBook>(m_ask, _price, _order))
            m_bid.at(_price).push_back(_order);
    } else {
        if (add<BidBook>(m_bid, _price, _order))
            m_ask.at(_price).push_back(_order);
    }
    return res;
}

void OrderBook::modify(OrderType _type, Price _price, Price _oprice, Order &_order)
{
    if (_type == OrderType::Ask)
        modify<BidBook>(m_bid, _price, _oprice, _order);
    else
        modify<AskBook>(m_ask, _price, _oprice, _order);
}