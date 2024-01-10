#include "Common/Core/Logger.hpp"
#include "Server/Network/Processor.hpp"

namespace net
{
    namespace tcp
    {
        bool processor(std::shared_ptr<net::tcp::Socket> _socket)
        {
            Logger::Log("[tcp::Processor] processing socket");
            return false;
        }
    }

    namespace udp
    {
        bool processor(std::shared_ptr<net::udp::Socket> _socket)
        {
            std::ignore = _socket;

            return true;
        }
    }
}