#include <algorithm>
#include <vector>

#include "Server/Core/OrderBook.hpp"

template<IsBook T>
bool OrderBook::add(T &_book, Price _price, Order &_order)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    for (auto &[_key, _val] : _book) {
        if (_key > _price)
            break;
        OrderList &ol = _book.at(_key);

        for (size_t i = 0; i < ol.size(); i++) {
            Order &order = ol.at(i);

            if (order.quantity == _order.quantity) {
                ol.erase(ol.begin() + i);
                return false;
            } else if (order.quantity < _order.quantity) {
                _order.quantity -= order.quantity;
                ol.erase(ol.begin() + i);
            } else {
                order.quantity -= _order.quantity;
                return false;
            }
        }
    }
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