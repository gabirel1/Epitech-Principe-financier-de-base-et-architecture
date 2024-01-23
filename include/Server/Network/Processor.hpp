#pragma once

#include <memory>

#include "Common/Core/meta.hpp"
#include "Common/Network/Socket.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

namespace net
{
    namespace tcp
    {
        [[nodiscard]] bool processor(ClientSocket &_socket, NetToAction &_serial, RawOutput &_error);
    }
}