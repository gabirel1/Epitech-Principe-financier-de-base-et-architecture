#include <numeric>

#include "OrderBook.hpp"

std::vector<double> &&OrderBook::getPrice(OrderType _type) const
{
    const Book &ref = ((_type == OrderType::Bid) ? m_bid : m_ask);
    std::vector<double> price(ref.size());
    size_t i = 0;

    for (auto [_price, _order] : ref)
        price[i++] = _price;
    return std::move(price);
}

bool OrderBook::contain(OrderType _type, Price _price) const
{
    return ((_type == OrderType::Bid) ? m_bid : m_ask).contains(_price);
}

const OrderList &OrderBook::getOrders(OrderType _type, Price _price) const
{
    return ((_type == OrderType::Bid) ? m_bid : m_ask)[_price];
}

Quantity OrderBook::sumQuantity(OrderType _type, Price _price) const
{
    const OrderList &ref = getOrders(_type, _price);

    return std::accumulate(ref.begin(), ref.end(), 0, std::plus<Quantity>());
}

bool OrderBook::bid_add(Price _price, Quantity &_quant, UserId &_user)
{
    // todo
}

bool OrderBook::bid_modify(Price _price, Quantity &_quant, UserId &_user)
{
    // todo
}

bool OrderBook::ask_add(Price _price, Quantity &_quant, UserId &_user)
{
    // todo
}

bool OrderBook::ask_modify(Price _price, Quantity &_quant, UserId &_user)
{
    // todo
}