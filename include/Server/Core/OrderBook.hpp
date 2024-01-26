#pragma once

#include <mutex>
#include <map>
#include <unordered_map>

#include "Common/Core/Order.hpp"
#include "Server/Core/meta.hpp"
#include "Common/Thread/Queue.hpp"

using AskBook = std::map<Price, OrderList, std::greater_equal<Price>>;
using BidBook = std::map<Price, OrderList, std::less_equal<Price>>;

class OrderBook
{
    public:
        struct Event
        {
            OrderId orderId;
            Quantity orgQty;
            OrderStatus status;
            Price price;
            Quantity quantity;
            OrderType side;
            UserId userId;
            bool sold;
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
            OrderId target;
            Order order;
        };

        using EventQueue = ts::Queue<Event>;

        OrderBook(const std::string &_name, EventQueue &_output);
        virtual ~OrderBook() = default;

        [[nodiscard]] bool add(OrderType _type, Price _price, Order &_order);
        [[nodiscard]] bool modify(OrderType _type, Price _price, Order &_order);
        [[nodiscard]] bool cancel(OrderType _type, OrderId _orderId, bool _event = true);

        // front only
        [[nodiscard]] std::vector<Price> getPrice(OrderType _type);
        [[nodiscard]] bool contain(OrderType _type, Price _price);
        [[nodiscard]] const OrderList &getOrders(OrderType _type, Price _price);
        [[nodiscard]] Quantity sumQuantity(OrderType _type, Price _price);

        [[nodiscard]] bool has(OrderType _type, OrderId _orderId) const;

    protected:
        template<IsBook T>
        using OrderIdMap = std::unordered_map<OrderId, std::pair<typename T::iterator, OrderList::iterator>>;

        void add(OrderType _type, Price _price, Order &_order, OrderStatus _status);

        template<IsBook T, class _T>
        bool add(T &_book, Price _price, Order &_order);
        template<IsBook T>
        bool cancel(OrderIdMap<T> &_mapId, OrderId _orderId, bool _event);

        template<IsBook T>
        [[nodiscard]] std::vector<Price> inter_getPrice(const T &_book);

    private:
        const std::string m_name;

        std::mutex m_mutex;

        OrderIdMap<BidBook> m_bid_id;
        OrderIdMap<AskBook> m_ask_id;

        EventQueue &m_output;

        BidBook m_bid;
        AskBook m_ask;
};

#include "Server/Core/OrderBook.inl"