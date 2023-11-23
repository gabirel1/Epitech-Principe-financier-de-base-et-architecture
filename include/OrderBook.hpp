#pragma once

#include <forward_list>
#include <unordered_map>
#include <vector>

using UserId = uint64_t;
using Quantity = uint64_t;
using Price = double;
using Order = std::pair<Quantity, UserId>;
using OrderList = std::forward_list<Order>;
using Book = std::unordered_map<Price, OrderList>;

enum class OrderType
{
    Bid,
    Ask
};

class OrderBook
{
    public:
        OrderBook() = default;
        virtual ~OrderBook() = default;

        // BID
        [[nodiscard]] bool bid_add(Price _price, Quantity &_quant, UserId &_user);
        [[nodiscard]] bool bid_modify(Price _price, Quantity &_quant, UserId &_user);

        // ASK
        [[nodiscard]] bool ask_add(Price _price, Quantity &_quant, UserId &_user);
        [[nodiscard]] bool ask_modify(Price _price, Quantity &_quant, UserId &_user);

        // front only
        [[nodiscard]] std::vector<Price> getPrice(OrderType _type) const;
        [[nodiscard]] bool contain(OrderType _type, Price _price) const;
        [[nodiscard]] const OrderList &getOrders(OrderType _type, Price _price) const;
        [[nodiscard]] Quantity sumQuantity(OrderType _type, Price _price) const;

    private:
        Book m_bid;
        Book m_ask;
};