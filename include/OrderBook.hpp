#pragma once

#include <forward_list>
#include <unordered_map>
#include <vector>

using UserId = uint64_t;
using Quantity = uint64_t;
using Price = double;
using Order = std::pair<Quantity, UserId>;

class OrderBook
{
    public:
        OrderBook() = default;
        virtual ~OrderBook() = default;

        // BID
        [[nodiscard]] std::vector<double> bid_getPrice() const; // front only
        [[nodiscard]] const std::forward_list<Order> &bid_getOrders(double);
        [[nodiscard]] uint64_t bidsumQuantity(double);

        [[nodiscard]] bool bid_add(Price _price, Quantity &_quant, UserId &_user);
        [[nodiscard]] bool bid_modify(Price _price, Quantity &_quant, UserId &_user);


        // ASK
        [[nodiscard]] std::vector<double> ask_getPrice() const; // front only
        [[nodiscard]] const std::forward_list<Order> &ask_getOrders(double) const;
        [[nodiscard]] uint64_t ask_sumQuantity(double) const;

        [[nodiscard]] bool ask_add(Price _price, Quantity &_quant, UserId &_user);
        [[nodiscard]] bool ask_modify(Price _price, Quantity &_quant, UserId &_user);

    private:
        std::unordered_map<Price, std::forward_list<Order>> m_bid;
        std::unordered_map<Price, std::forward_list<Order>> m_ask;
};