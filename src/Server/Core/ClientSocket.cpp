#include "Server/Core/ClientSocket.hpp"

ClientSocket::ClientSocket(std::shared_ptr<net::tcp::Socket> _socket)
    : m_socket(_socket)
{
}

std::shared_ptr<net::tcp::Socket> ClientSocket::getSocket() const
{
    return m_socket;
}

bool ClientSocket::operator==(const ClientSocket &_client) const
{
    return m_socket == _client.m_socket && Logged == _client.Logged && User == _client.User;
}

ClientSocket::operator bool() const
{
    return m_socket->is_open();
}
