#include <numeric>

#include "OrderBook.hpp"

std::vector<double> OrderBook::getPrice(OrderType _type) const
{
    const Book &ref = ((_type == OrderType::Bid) ? m_bid : m_ask);
    std::vector<double> price(ref.size());
    size_t i = 0;

    for (auto [_price, _order] : ref)
        price[i++] = _price;
    return price;
}

bool OrderBook::contain(OrderType _type, Price _price) const
{
    return ((_type == OrderType::Bid) ? m_bid : m_ask).contains(_price);
}

const OrderList &OrderBook::getOrders(OrderType _type, Price _price) const
{
    return ((_type == OrderType::Bid) ? m_bid : m_ask).at(_price);
}

Quantity OrderBook::sumQuantity(OrderType _type, Price _price) const
{
    const OrderList &ref = getOrders(_type, _price);

    return std::accumulate(ref.begin(), ref.end(), 0ull, [] (Quantity _total, const Order& _order) {
        return std::move(_total) + _order.first;
    });
}

bool OrderBook::bid_add(Price _price, Quantity &_quant, UserId &_user)
{
    // todo
    return false;
}

bool OrderBook::bid_modify(Price _price, Quantity &_quant, UserId &_user)
{
    // todo
    return false;
}

bool OrderBook::ask_add(Price _price, Quantity &_quant, UserId &_user)
{
    // todo
    return false;
}

bool OrderBook::ask_modify(Price _price, Quantity &_quant, UserId &_user)
{
    // todo
    return false;
}