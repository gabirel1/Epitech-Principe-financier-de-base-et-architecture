#pragma once

#include <mutex>
#include <map>

#include "Server/Core/Order.hpp"
#include "Server/Core/meta.hpp"

#include "Server/Core/Pipeline/Naming.hpp"

using AskBook = std::map<Price, OrderList, std::greater<Price>>;
using BidBook = std::map<Price, OrderList, std::less<Price>>;

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

        OrderBook(OBOutput &_output);
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
        template<IsBook T, class _T>
        bool add(T &_book, Price _price, Order& _order);
        template<IsBook T>
        void modify(T &_book, Price _price, Price _oprice, Order &_order);
        template<IsBook T>
        bool cancel(T& _book, Price _price, UserId _userId, OrderId _orderId);

        template<IsBook T>
        [[nodiscard]] std::vector<Price> inter_getPrice(const T &_book);

    private:
        std::mutex m_mutex;

        OBOutput &m_output;

        BidBook m_bid;
        AskBook m_ask;
};

#include "Server/Core/OrderBook.inl"