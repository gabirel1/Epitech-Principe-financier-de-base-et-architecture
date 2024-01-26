#include "Server/Core/Pipeline/Naming.hpp"

ActionInput::ActionInput(const ActionInput &&_data) noexcept
    : Client(std::move(_data.Client)), Message(std::move(_data.Message))
{
}

ActionInput::ActionInput(const ClientSocket &_client, const fix::Serializer::AnonMessage &&_msg) noexcept
    : Client(_client), Message(std::move(_msg))
{
}

ActionInput &ActionInput::operator=(ActionInput &&_data) noexcept
{
    if (this != &_data) {
        Client = std::move(_data.Client);
        Message = std::move(_data.Message);
    }
    return *this;
}

MarketInput::MarketInput(const MarketInput &&_data) noexcept
    : Client(std::move(_data.Client)), OrderData(std::move(_data.OrderData))
{
}

MarketInput::MarketInput(const MarketInput &_data)
    : Client(_data.Client), OrderData(_data.OrderData)
{
}

MarketInput::MarketInput(const ClientSocket &&_client) noexcept
    : Client(std::move(_client))
{
}

MarketInput &MarketInput::operator=(MarketInput &&_data) noexcept
{
    if (this != &_data) {
        Client = std::move(_data.Client);
        OrderData = std::move(_data.OrderData);
    }
    return *this;
}

OutNetworkInput::OutNetworkInput(const OutNetworkInput &&_data) noexcept
    : Client(std::move(_data.Client)), Message(std::move(_data.Message))
{
}

OutNetworkInput::OutNetworkInput(const OutNetworkInput &_data)
    : Client(_data.Client), Message(_data.Message)
{
}

OutNetworkInput::OutNetworkInput(const ClientSocket &&_client, const fix::Message &&_msg) noexcept
    : Client(std::move(_client)), Message(std::move(_msg))
{
}

OutNetworkInput::OutNetworkInput(const ClientSocket &_client, const fix::Message &_msg)
    : Client(_client), Message(_msg)
{
}

OutNetworkInput &OutNetworkInput::operator=(OutNetworkInput &&_data) noexcept
{
    if (this != &_data) {
        Client = std::move(_data.Client);
        Message = std::move(_data.Message);
    }
    return *this;
}

MarketContainerQueue::MarketContainerQueue(InMarket &_market, InMarketData &_data)
    : m_market(_market), m_data(_data)
{
}

void MarketContainerQueue::pushToProcess(const MarketInput &&_data)
{
    m_market.push(std::move(_data));
}

void MarketContainerQueue::pushToData(const MarketDataInput &&_data)
{
    m_data.push(std::move(_data));
}