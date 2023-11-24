#pragma once

#include <map>
#include <vector>
#include <set>

using UserId = uint64_t;
using OrderId = uint64_t;
using Quantity = uint64_t;

struct Order
{
    UserId userId;
    OrderId orderId;
    Quantity quantity;
};

using Price = double;
using OrderList = std::vector<Order>;
using Book = std::map<Price, OrderList>;
using PriceSet = std::set<Price>;

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

        bool add(OrderType _type, Price _price, Order &_order);
        [[nodiscard]] bool modify(OrderType _type, Price _price, Order &_order);

        // front only
        [[nodiscard]] std::vector<Price> getPrice(OrderType _type) const;
        [[nodiscard]] bool contain(OrderType _type, Price _price) const;
        [[nodiscard]] const OrderList &getOrders(OrderType _type, Price _price) const;
        [[nodiscard]] Quantity sumQuantity(OrderType _type, Price _price) const;

    protected:
        static bool add(Book& _book, Price _price, Order &_order);

    private:
        Book m_bid;
        Book m_ask;
};