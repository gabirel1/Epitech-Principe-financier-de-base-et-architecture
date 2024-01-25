#include <algorithm>
#include <vector>

#include "Common/Core/Logger.hpp"
#include "Server/Core/OrderBook.hpp"

template<IsBook T, class _T>
bool OrderBook::add(T &_book, Price _price, Order &_order)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    _T cmp{};
    Event event;

    if constexpr (std::is_same_v<T, BidBook>)
        event.side = OrderType::Ask;
    else
        event.side = OrderType::Bid;
    event.sold = true;
    for (auto &[_key, _val] : _book) {
        if (cmp(_key, _price))
            break;

        event.price = _key;
        std::cout << "event side: " << ((event.side == OrderType::Bid) ? "Bid" : "Ask") << "\n\n\n\n" <<std::endl;
        for (size_t i = 0; i < _val.size(); i++) {
            Order &order = _val.at(i);

            if (order.userId == _order.userId)
                continue;
            event.orderId = order.orderId;
            event.status = OrderStatus::Filled;
            event.userId = order.userId;
            event.quantity = 0;
            event.orgQty = order.quantity;
            std::cout << "here" << std::endl;
            if (order.quantity == _order.quantity) {
                Logger::Log("[OrderBook] (", m_name, ") {Add} Filled the order: ", order, ", price: ", _key);
                Logger::Log("[OrderBook] (", m_name, ") {Add-Incoming} Filled the order: ", _order);
                _val.erase(_val.begin() + i);
                _order.quantity = 0;
                m_output.append(event);
                return false;
            } else if (order.quantity < _order.quantity) {
                Logger::Log("[OrderBook] (", m_name, ") {Add} Filled the order: ", order, ", price: ", _key);
                Logger::Log("[OrderBook] (", m_name, ") {Add-Incoming} Partially filled the order: ", _order, ", new quantity: ", _order.quantity - order.quantity);
                _order.quantity -= order.quantity;
                _val.erase(_val.begin() + i);
                m_output.append(event);
            } else {
                Logger::Log("[OrderBook] (", m_name, ") {Add} Partially sold the order: ", order, ", new quantity: ", order.quantity - _order.quantity, ", price: ", _key);
                Logger::Log("[OrderBook] (", m_name, ") {Add-Incoming} Filled the order: ", _order);
                order.quantity -= _order.quantity;
                event.quantity = order.quantity;
                event.status = OrderStatus::PartiallyFilled;
                m_output.append(event);
                return false;
            }
        }
    }
    Logger::Log("[OrderBook] (Add) Finished order processing: ", _order, ", price: ", _price);
    return true;
}

template<IsBook T>
bool OrderBook::cancel(OrderIdMap<T> &_mapId, OrderId _orderId, bool _event)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    typename OrderIdMap<T>::iterator it = _mapId.find(_orderId);

    if (it != _mapId.end()) {
        if (_event) {
            Event event;

            if constexpr (std::is_same_v<T, BidBook>)
                event.side = OrderType::Bid;
            else
                event.side = OrderType::Ask;
            event.orderId = _orderId;
            event.status = OrderStatus::Canceld;
            event.userId = it->second.second->userId;
            event.quantity = it->second.second->quantity;
            event.orgQty = it->second.second->quantity;
            event.sold = false;
            Logger::Log("[OrderBook] (", m_name, ") {Cancel} Sended event: "); // todo log
            m_output.append(event);
        }
        Logger::Log("[OrderBook] (", m_name, ") {Cancel} Sucefully canceled the order: ", _orderId);
        it->second.first->second.erase(it->second.second);
        _mapId.erase(it);
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