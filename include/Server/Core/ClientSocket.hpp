#pragma once

#include <memory>

#include "Common/Network/Socket.hpp"
#include "Server/Core/Order.hpp"

class ClientSocket
{
    public:
        ClientSocket(std::shared_ptr<net::tcp::Socket> _socket = nullptr);
        ~ClientSocket() = default;

        [[nodiscard]] std::shared_ptr<net::tcp::Socket> getSocket() const;

        bool Logged = false;
        bool Disconnect = false;
        UserId User = 0;
        size_t SeqNumber = 1;

        bool operator==(const ClientSocket &_client) const;
        operator bool() const;

    protected:
        std::shared_ptr<net::tcp::Socket> m_socket;
};