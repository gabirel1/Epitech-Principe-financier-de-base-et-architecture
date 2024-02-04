#include "Client/Context.hpp"

void Context::reset()
{
    Loggin = false;
    User = "";
    HeartBit = 0;
    SeqNum = 1;
    userInfos.reset();
}

std::ostream &operator<<(std::ostream &_os, const Context &_ctx)
{
    _os << "Context:\n";
    _os << "\t- Logon: " << _ctx.Loggin << "\n";
    _os << "\t- SeqNUm: " << _ctx.SeqNum << "\n";
    _os << "\t- UserId: " << _ctx.User << "\n";
    _os << "\t- HeartBit: " << _ctx.HeartBit << std::endl;
    _os << "\t- MyOrders: " << std::endl;
    _os << _ctx.userInfos.getOrders();
    return _os;
}
