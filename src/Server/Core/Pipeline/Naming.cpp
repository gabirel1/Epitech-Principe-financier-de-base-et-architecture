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

    MarketToNet &MarketToNet::operator=(const MarketToNet &_data)
    {
        Client = _data.Client;
        Message = _data.Message;
        return *this;
    }
}