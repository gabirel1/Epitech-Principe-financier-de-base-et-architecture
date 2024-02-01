#include "Client/Context.hpp"

void Context::reset()
{
    Loggin = false;
    User = "";
    HeartBit = 0;
    SeqNum = 1;
    MyOrders.clear();
}

std::ostream &operator<<(std::ostream &_os, const Context &_ctx)
{
    _os << "Context:\n";
    _os << "\t- Logon: " << _ctx.Loggin << "\n";
    _os << "\t- SeqNUm: " << _ctx.SeqNum << "\n";
    _os << "\t- UserId: " << _ctx.User << "\n";
    _os << "\t- HeartBit: " << _ctx.HeartBit << std::endl;
    _os << "\t- MyOrders: " << std::endl;
    for (auto &_order : _ctx.MyOrders)
        _os << "\t\t- " << "[" << ((_order.type == OrderType::Bid) ? "Buy" : "Sell") << "]orderId: '" << _order.orderId << "', quantity: '" << _order.quantity << "', price: '" << _order.price << "', status: '" << (int)_order.status << "'" << std::endl;
    return _os;
}