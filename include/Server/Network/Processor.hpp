#pragma once

#include <memory>

#include "Common/Core/meta.hpp"
#include "Common/Network/Socket.hpp"

namespace net
{
    namespace tcp
    {
        [[nodiscard]] bool processor(std::shared_ptr<net::tcp::Socket> _socket);
    }

    namespace udp
    {
        [[nodiscard]] bool processor(std::shared_ptr<net::udp::Socket> _socket);
    }
}