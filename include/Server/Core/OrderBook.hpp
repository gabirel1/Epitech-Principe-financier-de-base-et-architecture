#pragma once

#include <mutex>
#include <map>

#include "Server/Core/Order.hpp"
#include "Server/Core/meta.hpp"
#include "Common/Thread/Queue.hpp"

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
        struct Event
        {
            OrderId orderId;
            OrderStatus status;
            Price price;
            Quantity quantity;
            Quantity orgQty;
            Side side;
        };

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
            Price oprice;
            Order order;
        };

        using EventQueue = ts::Queue<Event>;

        OrderBook(EventQueue &_output);
        virtual ~OrderBook() = default;

        bool add(OrderType _type, Price _price, Order& _order);
        bool modify(OrderType _type, Price _price, Order &_order);
        bool cancel(OrderType _type, Order &_order);

        // front only
        [[nodiscard]] std::vector<Price> getPrice(OrderType _type);
        [[nodiscard]] bool contain(OrderType _type, Price _price);
        [[nodiscard]] const OrderList &getOrders(OrderType _type, Price _price);
        [[nodiscard]] Quantity sumQuantity(OrderType _type, Price _price);

    protected:
        template<IsBook T, class _T>
        bool add(T &_book, Price _price, Order &_order);
        template<IsBook T>
        bool modify(T &_book, Price _price, Order &_order);
        template<IsBook T>
        bool cancel(T& _book, Order &_order);

        template<IsBook T>
        [[nodiscard]] std::vector<Price> inter_getPrice(const T &_book);

    private:
        std::mutex m_mutex;

        EventQueue &m_output;

        BidBook m_bid;
        AskBook m_ask;
};

#include "Server/Core/OrderBook.inl"