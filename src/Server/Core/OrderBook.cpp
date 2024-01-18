#include <numeric>

#include "Server/Core/OrderBook.hpp"

OrderBook::OrderBook(ts::Queue<Event> &_output)
    : m_output(_output)
{
}

bool OrderBook::add(OrderType _type, Price _price, Order& _order)
{
    bool res = false;

    if (_type == OrderType::Bid) {
        if (add<AskBook, std::less<Price>>(m_ask, _price, _order)) {
            std::lock_guard<std::mutex> guard(m_mutex);

            m_bid[_price].push_back(_order);

            res = true;
        }
    }
    else {
        if (add<BidBook, std::greater<Price>>(m_bid, _price, _order)) {
            std::lock_guard<std::mutex> guard(m_mutex);

            m_ask.at(_price).push_back(_order);
            res = true;
        }
    }
    return res;
}

bool OrderBook::modify(OrderType _type, Price _price, Order &_order)
{
    if (_type == OrderType::Ask)
        return modify<BidBook>(m_bid, _price, _order);
    else
        return modify<AskBook>(m_ask, _price, _order);
}

bool OrderBook::cancel(OrderType _type, Order &_order)
{
    if (_type == OrderType::Ask)
        return cancel<AskBook>(m_ask, _order);
    return cancel<BidBook>(m_bid, _order);
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