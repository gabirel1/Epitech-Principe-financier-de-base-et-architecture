#include "Server/Core/Pipeline/Naming.hpp"

namespace data
{
    NetToAction::NetToAction(const NetToAction &&_data) noexcept
        : Client(std::move(_data.Client)), Message(std::move(_data.Message))
    {
    }

    NetToAction::NetToAction(const ClientSocket &_client, const fix::Serializer::AnonMessage &&_msg) noexcept
        : Client(_client), Message(std::move(_msg))
    {
    }

    NetToAction &NetToAction::operator=(NetToAction &&_data) noexcept
    {
        if (this != &_data) {
            Client = std::move(_data.Client);
            Message = std::move(_data.Message);
        }
        return *this;
    }

    ActionToMarket::ActionToMarket(const ActionToMarket &&_data) noexcept
        : Client(std::move(_data.Client)), OrderData(std::move(_data.OrderData))
    {
    }

    ActionToMarket::ActionToMarket(const ActionToMarket &_data)
        : Client(_data.Client), OrderData(_data.OrderData)
    {
    }

    ActionToMarket::ActionToMarket(const ClientSocket &&_client) noexcept
        : Client(std::move(_client))
    {
    }

    ActionToMarket &ActionToMarket::operator=(ActionToMarket &&_data) noexcept
    {
        if (this != &_data) {
            Client = std::move(_data.Client);
            OrderData = std::move(_data.OrderData);
        }
        return *this;
    }

    MarketToNet::MarketToNet(const MarketToNet &&_data) noexcept
        : Client(std::move(_data.Client)), Message(std::move(_data.Message))
    {
    }

    MarketToNet::MarketToNet(const MarketToNet &_data)
        : Client(_data.Client), Message(_data.Message)
    {
    }

    MarketToNet::MarketToNet(const ClientSocket &&_client, const fix::Message &&_msg) noexcept
        : Client(std::move(_client)), Message(std::move(_msg))
    {
    }

    MarketToNet::MarketToNet(const ClientSocket &_client, const fix::Message &_msg)
        : Client(_client), Message(_msg)
    {
    }

    MarketToNet &MarketToNet::operator=(MarketToNet &&_data) noexcept
    {
        if (this != &_data) {
            Client = std::move(_data.Client);
            Message = std::move(_data.Message);
        }
        return *this;
    }
}