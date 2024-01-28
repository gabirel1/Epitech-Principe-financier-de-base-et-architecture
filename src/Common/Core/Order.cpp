#include "Common/Core/Order.hpp"

std::ostream &operator<<(std::ostream &_os, const Order &_order)
{
    _os << "{ Order-id: " << _order.orderId << ", Quantity: " << _order.quantity << ", User-id: " << _order.userId << " }";
    return _os;
}

std::istream &operator>>(std::istream &_is, OrderType &_type)
{
    // warning maybe uint8_t is wrong type
    uint8_t type = 0;

    _is >> _type;
    _type = static_cast<OrderType>(type);
    return _is;
}