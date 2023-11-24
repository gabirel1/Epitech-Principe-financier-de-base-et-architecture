#pragma once

#include <unordered_map>
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
using Book = std::unordered_map<Price, OrderList>;
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

        // BID
        bool bid_add(Price _price, Order &_order);
        [[nodiscard]] bool bid_modify(Price _price, Order &_order);

        // ASK
        bool ask_add(Price _price, Order &_order);
        [[nodiscard]] bool ask_modify(Price _price, Order &_order);

        // front only
        [[nodiscard]] std::vector<Price> getPrice(OrderType _type) const;
        [[nodiscard]] bool contain(OrderType _type, Price _price) const;
        [[nodiscard]] const OrderList &getOrders(OrderType _type, Price _price) const;
        [[nodiscard]] Quantity sumQuantity(OrderType _type, Price _price) const;

    protected:
        bool add(Book& _book, PriceSet& _ps, Price _price, Order &_order);

    private:
        Book m_bid;
        PriceSet m_bid_price;
        Book m_ask;
        PriceSet m_ask_price;
};