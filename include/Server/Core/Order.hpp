#pragma once

#include <cstdint>
#include <vector>

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
