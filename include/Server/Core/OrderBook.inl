#include <algorithm>
#include <vector>

#include "Server/Core/OrderBook.hpp"

// OrdType (40) = 1 Market
// Price (44)
// LeavesQty (151) = remaing
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
    data::MarketEvent event;

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
            event.remaing = 0;
            if (order.quantity == _order.quantity) {
                ol.erase(ol.begin() + i);
                m_output.append(event);
                return false;
            } else if (order.quantity < _order.quantity) {
                _order.quantity -= order.quantity;
                ol.erase(ol.begin() + i);
                m_output.append(event);
            } else {
                order.quantity -= _order.quantity;
                event.remaing = order.quantity;
                event.status = OrderStatus::PartiallyFilled;
                m_output.append(event);
                return false;
            }
        }
    }
    return true;
}

template<IsBook T>
void OrderBook::modify(T &_book, Price _price, Price _oprice, Order &_order)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    OrderList &loprice = _book.at(_oprice);
    auto it = std::find_if(loprice.begin(), loprice.end(), [_order] (const Order &_iorder) {
        return _iorder.orderId == _order.orderId && _iorder.userId == _order.userId;
    });

    if (_price == _oprice) {
        it->quantity = _order.quantity;
    } else {
        OrderList &lprice = _book.at(_oprice);

        lprice.emplace_back(std::move(*it));
        loprice.erase(it);
    }
}

template<IsBook T>
bool OrderBook::cancel(T& _book, Price _price, UserId _userId, OrderId _orderId)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    if (_book.contains(_price)) {
        OrderList &ol = _book.at(_price);

        std::erase_if(ol, [_userId, _orderId] (const Order& _order) {
            return _order.userId == _userId && _order.orderId == _orderId;
        });
        return true;
    }
    return false;
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