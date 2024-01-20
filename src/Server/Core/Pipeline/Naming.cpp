#include "Server/Core/Pipeline/Naming.hpp"

namespace data
{
    NetToAction::NetToAction(const NetToAction &_data)
        : Client(_data.Client), Message(_data.Message)
    {
    }

    NetToAction::NetToAction(const ClientSocket &_client, const fix::Serializer::AnonMessage &_msg)
        : Client(_client), Message(_msg)
    {
    }

    NetToAction &NetToAction::operator=(const NetToAction &_data)
    {
        Client = _data.Client;
        Message = _data.Message;
        return *this;
    }

    ActionToMarket::ActionToMarket(const ActionToMarket &_data)
        : Client(_data.Client), OrderData(_data.OrderData)
    {
    }

    ActionToMarket &ActionToMarket::operator=(const ActionToMarket &_data)
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
}