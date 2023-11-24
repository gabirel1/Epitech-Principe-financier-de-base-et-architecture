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

bool OrderBook::bid_add(Price _price, Order &_order)
{
    for (const Price &_aprice : m_ask_price) {
        if (_aprice > _price)
            break;
        OrderList &ol = m_ask.at(_aprice);

        for (size_t i = 0; i < ol.size(); i++) {
            Order &order = ol.at(i);

            if (order.quantity == _order.quantity) {
                ol.erase(i);
                return true;
            } else if (order.quantity < _order.quantity) {
                _order.quantity -= order.quantity;
                ol.erase(i);
            } else {
                order.quantity -= _order.quantity;
                return true;
            }
        }
    }
    m_bid.at(_price).push_back(_order);
    return false;
}

bool OrderBook::bid_modify(Price _price, Order &_order)
{
    // todo
    return false;
}

bool OrderBook::ask_add(Price _price, Order &_order)
{
    for (const Price &_aprice : m_bid_price) {
        if (_aprice > _price)
            break;
        OrderList &ol = m_bid.at(_aprice);

        for (size_t i = 0; i < ol.size(); i++) {
            Order &order = ol.at(i);

            if (order.quantity == _order.quantity) {
                ol.erase(i);
                return true;
            } else if (order.quantity < _order.quantity) {
                _order.quantity -= order.quantity;
                ol.erase(i);
            } else {
                order.quantity -= _order.quantity;
                return true;
            }
        }
    }
    m_bid.at(_price).push_back(_order);
    return false;
}

bool OrderBook::ask_modify(Price _price, Order &_order)
{
    // todo
    return false;
}