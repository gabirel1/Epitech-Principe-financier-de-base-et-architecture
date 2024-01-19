#include "Server/Core/Pipeline/Naming.hpp"

namespace data
{
    NetToSerial::NetToSerial(const NetToSerial &_data)
        : Client(_data.Client), Message(_data.Message)
    {
    }

    NetToSerial::NetToSerial(const ClientSocket &_client, const fix::Serializer::AnonMessage &_msg)
        : Client(_client), Message(_msg)
    {
    }

    NetToSerial &NetToSerial::operator=(const NetToSerial &_data)
    {
        Client = _data.Client;
        Message = _data.Message;
        return *this;
    }

    SerialToMarket::SerialToMarket(const SerialToMarket &_data)
        : Client(_data.Client), OrderData(_data.OrderData)
    {
    }

    SerialToMarket &SerialToMarket::operator=(const SerialToMarket &_data)
    {
        Client = _data.Client;
        OrderData = _data.OrderData;
        return *this;
    }

    MarketToNet::MarketToNet(const MarketToNet &_data)
        : Client(_data.Client), Message(_data.Message)
    {
    }

    MarketToNet::MarketToNet(const ClientSocket &_client, const fix::Message &_msg)
        : Client(_client), Message(_msg)
    {
    }


    MarketToNet &MarketToNet::operator=(const MarketToNet &_data)
    {
        Client = _data.Client;
        Message = _data.Message;
        return *this;
    }

    std::ostream &operator<<(std::ostream &_os, const UDPPackage &_packeage)
    {
        _os << "{ id: " << _packeage.id <<  ", prcie: " << _packeage.price << ", quantity: " << _packeage.quantity;
        _os << ", book: " << ((UDP_FLAG_GET_BOOK(_packeage.flag) == OrderType::Ask) ? "Ask" : "Bid");
        _os << ", status: " << UDP_FLAG_GET_STATUS(_packeage.flag);
        _os << ", is-sold: " << UDP_FLAG_GET_IS_SOLD(_packeage.flag) << " }";
        return _os;
    }
}