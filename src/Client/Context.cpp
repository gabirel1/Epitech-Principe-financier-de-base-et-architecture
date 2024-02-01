#include "Client/Context.hpp"

std::ostream &operator<<(std::ostream &_os, const Context &_ctx)
{
    _os << "Context:\n";
    _os << "\t- Logon: " << _ctx.Loggin << "\n";
    _os << "\t- SeqNUm: " << _ctx.SeqNum << "\n";
    _os << "\t- UserId: " << _ctx.User << "\n";
    _os << "\t- HeartBit: " << _ctx.HeartBit << std::endl;
    return _os;
}