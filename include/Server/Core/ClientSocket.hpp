#pragma once

#include <memory>

#include "Server/Core/OrderBook.hpp"
#include "Common/Network/Socket.hpp"

class ClientSocket
{
    public:
        ClientSocket(std::shared_ptr<net::tcp::Socket> _socket = nullptr);
        ~ClientSocket() = default;

        [[nodiscard]] std::shared_ptr<net::tcp::Socket> getSocket() const;

        bool Logged = false;
        UserId User = 0;

        bool operator==(const ClientSocket &_client) const;
        operator bool() const;

    protected:
        std::shared_ptr<net::tcp::Socket> m_socket;
};