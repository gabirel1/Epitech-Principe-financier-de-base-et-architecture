#include "OrderBook.hpp"


template<class T>
bool OrderBook::add(T &_book, Price _price, Order &_order)
{
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

template<class T>
std::vector<double> OrderBook::getPrice(const T &_book)
{
    std::vector<double> price(_book.size());
    size_t i = 0;

    for (auto [_price, _order] : _book)
        price[i++] = _price;
    return price;
}