#include "Server/Core/ClientSocket.hpp"

ClientSocket::ClientSocket(std::shared_ptr<net::tcp::Socket> _socket)
    : m_socket(_socket)
{
}

ClientSocket::ClientSocket(const ClientSocket &_client)
    : Logged(_client.Logged), Disconnect(_client.Disconnect),
        User(_client.User), SeqNumber(_client.SeqNumber),
        m_request(_client.m_request), m_socket(_client.m_socket)
{
}

ClientSocket::ClientSocket(const ClientSocket &&_client) noexcept
    : Logged(std::move(_client.Logged)), Disconnect(std::move(_client.Disconnect)),
        User(std::move(_client.User)), SeqNumber(std::move(_client.SeqNumber)),
        m_socket(std::move(_client.m_socket))
{
}

std::shared_ptr<net::tcp::Socket> ClientSocket::getSocket() const
{
    return m_socket;
}

void ClientSocket::newRequest()
{
    m_request.emplace(SeqNumber, std::chrono::system_clock::now());
}

bool ClientSocket::hasRequest(size_t _seqNumber) const
{
    return m_request.contains(_seqNumber);
}

std::chrono::system_clock::time_point ClientSocket::getRequest(size_t _seqNumber)
{
    auto it = m_request.find(_seqNumber);

    if (it == m_request.end())
        return std::chrono::system_clock::now();
    auto ret = it->second;
    m_request.erase(it);
    return ret;
}

ClientSocket &ClientSocket::operator=(ClientSocket &&_client) noexcept
{
    if (this != &_client) {
        Logged = std::move(_client.Logged);
        Disconnect = std::move(_client.Disconnect);
        User = std::move(_client.User);
        SeqNumber = std::move(_client.SeqNumber);
        m_socket = std::move(_client.m_socket);
    }
    return *this;
}

bool ClientSocket::operator==(const ClientSocket &_client) const
{
    return m_socket == _client.m_socket && Logged == _client.Logged && User == _client.User;
}

ClientSocket::operator bool() const
{
    return m_socket->is_open();
}
