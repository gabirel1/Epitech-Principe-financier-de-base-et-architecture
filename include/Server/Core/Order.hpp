#pragma once

#include <cstdint>
#include <vector>

using UserId = uint64_t;
using OrderId = uint64_t;
using Quantity = uint64_t;
using Side = uint8_t;

struct Order
{
    UserId userId;
    OrderId orderId;
    Quantity quantity;
};

using Price = double;
using OrderList = std::vector<Order>;

/// @brief Available value for OrdStatus [39](https://www.onixs.biz/fix-dictionary/4.2/tagNum_39.html).
enum OrderStatus {
    New = 0,
    PartiallyFilled,
    Filled,
    Canceld = 4,
    Replaced
};