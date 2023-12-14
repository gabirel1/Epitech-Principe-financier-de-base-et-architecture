#pragma once

#include <memory>

#include "Core/OrderBook.hpp"
#include "Network/Socket.hpp"

class ClientSocket
{
    public:
        ClientSocket(std::shared_ptr<net::tcp::Socket> _socket);
        ~ClientSocket() = default;

        [[nodiscard]] std::shared_ptr<net::tcp::Socket> getSocket() const;

        bool Logged = false;
        UserId User = 0;

    protected:
        std::shared_ptr<net::tcp::Socket> m_socket;
};