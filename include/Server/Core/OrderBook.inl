#include <algorithm>
#include <vector>

#include "Server/Core/OrderBook.hpp"

#include <iostream> // todo remove

// OrdType (40) = 1 Market
// Price (44)
// LeavesQty (151) = remaining
// ExecID = global Id of the market?
// OrdStatus (39) = status
// Symbol (55) = market symbol
// OrderId

// AvgPx (6) = ????

template<IsBook T, class _T>
bool OrderBook::add(T &_book, Price _price, Order &_order)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    _T cmp{};
    Event event;

    if constexpr (std::is_same_v<T, BidBook>)
        event.side = 4;
    else
        event.side = 3;
    for (auto &[_key, _val] : _book) {
        if (cmp(_key, _price))
            break;
        OrderList &ol = _book.at(_key);

        event.price = _key;
        for (size_t i = 0; i < ol.size(); i++) {
            Order &order = ol.at(i);

            event.orderId = order.orderId;
            event.status = OrderStatus::Filled;
            event.quantity = 0;
            if (order.quantity == _order.quantity) {
                ol.erase(ol.begin() + i);
                std::cout << "add: append quantity == " << std::endl; // todo remove
                m_output.append(event);
                std::cout << "add: returning false" << std::endl; // todo remove
                return false;
            } else if (order.quantity < _order.quantity) {
                _order.quantity -= order.quantity;
                ol.erase(ol.begin() + i);
                std::cout << "add: append < quantity" << std::endl; // todo remove
                m_output.append(event);
            } else {
                order.quantity -= _order.quantity;
                event.quantity = order.quantity;
                event.status = OrderStatus::PartiallyFilled;
                std::cout << "add: append else" << std::endl; // todo remove
                m_output.append(event);
                std::cout << "add: returning false" << std::endl; // todo remove
                return false;
            }
        }
    }
    std::cout << "add: returning true" << std::endl; // todo remove
    return true;
}

template<IsBook T>
bool OrderBook::modify(T &_book, Price _price, Order &_order)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    Order order = _order;

    auto it = std::find_if(_book.begin(), _book.end(), [_order] (const T::value_type &_lorder) {
        return std::find_if(_lorder.second.begin(), _lorder.second.end(), [_order] (const Order &_iorder) {
            return _iorder.orderId == _order.orderId && _iorder.userId == _order.userId;
        }) != _lorder.second.end();
    });

    if (it == _book.end())
        return false;
    auto order_it = std::find_if(it->second.begin(), it->second.end(), [_order] (const Order &_iorder) {
        return _iorder.orderId == _order.orderId && _iorder.userId == _order.userId;
    });
    it->second.erase(order_it);
    _book.at(_price).emplace_back(order);
    return true;
}

template<IsBook T>
bool OrderBook::cancel(T &_book, Order &_order)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    return std::erase_if(_book, [_order] (T::value_type &_lorder) {
            return std::remove_if(_lorder.second.begin(), _lorder.second.end(), [_order] (Order &_iorder) {
                return _iorder.orderId == _order.orderId && _iorder.userId == _order.userId;
            }) != _lorder.second.end();
        }) != 0;
}

template<IsBook T>
std::vector<Price> OrderBook::inter_getPrice(const T &_book)
{
    std::vector<Price> price(_book.size());
    size_t i = 0;
    std::lock_guard<std::mutex> guard(m_mutex);

    for (auto [_price, _order] : _book)
        price[i++] = _price;
    return price;
}