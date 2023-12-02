#pragma once

#include <map>
#include <mutex>
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
        struct Data
        {
            enum class Action
            {
                Add,
                Modify,
                Cancel
            };

            Action action;
            OrderType type;
            Price price;
            UserId userId;
            OrderId orderId;
            Price oprice;
            Order order;
        };

        OrderBook() = default;
        virtual ~OrderBook() = default;

        bool add(OrderType _type, Price _price, Order& _order);
        void modify(OrderType _type, Price _price, Price _oprice, Order &_order);
        bool cancel(OrderType _type, Price _price, UserId _userId, OrderId _orderId);

        // front only
        [[nodiscard]] std::vector<Price> getPrice(OrderType _type);
        [[nodiscard]] bool contain(OrderType _type, Price _price);
        [[nodiscard]] const OrderList &getOrders(OrderType _type, Price _price);
        [[nodiscard]] Quantity sumQuantity(OrderType _type, Price _price);

    protected:
        template<class T>
        bool add(T &_book, Price _price, Order& _order);
        template<class T>
        void modify(T &_book, Price _price, Price _oprice, Order &_order);
        template<class T>
        bool cancel(T& _book, Price _price, UserId _userId, OrderId _orderId);

        template<class T>
        [[nodiscard]] std::vector<Price> inter_getPrice(const T &_book);

    private:
        std::mutex m_mutex;

        BidBook m_bid;
        AskBook m_ask;
};

#include "Core/OrderBook.inl"