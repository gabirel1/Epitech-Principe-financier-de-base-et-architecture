#include "Core/Pipeline/Naming.hpp"

namespace data
{
    NetToSerial::NetToSerial(NetToSerial &&_data) noexcept
        : Client(std::move(_data.Client)), Message(std::move(_data.Message))
    {
    }

    MarketToNet::MarketToNet(const MarketToNet &&_data) noexcept
        : Client(std::move(_data.Client)), Message(std::move(_data.Message))
    {
    }
}