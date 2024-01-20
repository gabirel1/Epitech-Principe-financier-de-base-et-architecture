#pragma once

#include <memory>

#include "Common/Core/meta.hpp"
#include "Common/Network/Socket.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

namespace net
{
    namespace tcp
    {
        [[nodiscard]] bool processor(ClientSocket &_socket, NetToSerial &_serial, RawOutput &_error);
    }

    namespace udp
    {
        [[nodiscard]] bool processor(std::shared_ptr<net::udp::Socket> _socket);
    }
}