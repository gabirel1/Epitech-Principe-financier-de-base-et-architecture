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
using AskBook = std::map<Price, OrderList, std::greater<Price>>;
using BidBook = std::map<Price, OrderList, std::less<Price>>;
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

        bool add(OrderType _type, Price _price, Order& _order);
        [[nodiscard]] bool modify(OrderType _type, Price _price, Order& _order);

        // front only
        [[nodiscard]] std::vector<Price> getPrice(OrderType _type) const;
        [[nodiscard]] bool contain(OrderType _type, Price _price) const;
        [[nodiscard]] const OrderList& getOrders(OrderType _type, Price _price) const;
        [[nodiscard]] Quantity sumQuantity(OrderType _type, Price _price) const;

    protected:
        template<class T>
        static bool add(T& _book, Price _price, Order& _order);

        template<class T>
        [[nodiscard]] static std::vector<double> getPrice(const T &_book);

    private:
        BidBook m_bid;
        AskBook m_ask;
};

#include "OrderBook.inl"