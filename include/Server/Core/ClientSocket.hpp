#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>

#include "Common/Core/Order.hpp"
#include "Common/Network/Socket.hpp"

class ClientSocket
{
    public:
        ClientSocket(std::shared_ptr<net::tcp::Socket> _socket = nullptr);
        ClientSocket(const ClientSocket &_client);
        ClientSocket(const ClientSocket &&_client) noexcept;
        ~ClientSocket() = default;

        [[nodiscard]] std::shared_ptr<net::tcp::Socket> getSocket() const;

        bool Logged = false;
        bool Disconnect = false;
        UserId User = "";
        size_t SeqNumber = 1;

        void newRequest();
        [[nodiscard]] bool hasRequest(size_t _seqNumber) const;
        std::chrono::system_clock::time_point getRequest(size_t _seqNumber);

        void subscribe(const std::string &_symbol, uint8_t _subtype);
        [[nodiscard]] bool isSubscribe(const std::string &_symbol, uint8_t _subtype);

        ClientSocket &operator=(ClientSocket &&_client) noexcept;

        bool operator==(const ClientSocket &_client) const;
        operator bool() const;

    protected:
        std::unordered_map<size_t, std::chrono::system_clock::time_point> m_request;
        std::shared_ptr<net::tcp::Socket> m_socket;
        std::unordered_map<std::string, uint8_t> m_subscribe;
};