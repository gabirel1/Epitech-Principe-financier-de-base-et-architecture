#include "Server/Core/ClientSocket.hpp"

ClientSocket::ClientSocket(std::shared_ptr<net::tcp::Socket> _socket)
    : m_socket(_socket)
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

bool ClientSocket::operator==(const ClientSocket &_client) const
{
    return m_socket == _client.m_socket && Logged == _client.Logged && User == _client.User;
}

ClientSocket::operator bool() const
{
    return m_socket->is_open();
}
