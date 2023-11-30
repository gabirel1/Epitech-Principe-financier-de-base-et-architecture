#include "Network/Socket.hpp"

namespace net
{
    void Socket::blocking(bool _block)
    {
        m_block = _block;
    }

    bool Socket::isBlocking() const
    {
        return m_block;
    }

    bool Socket::connect(const Ip &_ip, uint32_t _port)
    {
        struct sockaddr_in addrs;

        if (!create(AF_INET, 0) && connect(m_fd, (struct sockaddr *)&addrs, sizeof(addrs)) < 0)
            return false;
        memset(&addrs, 0, sizeof(addrs));
        addrs.sin_family = AF_INET;
        addrs.sin_port = htons(_port);
        if (inet_pton(AF_INET, _ip.c_str(), &addrs.sin_addr) <= 0 &&
            ::connect(m_socket, (struct sockaddr *)_addrs, sizeof(addrs)) < 0)
            return false;
        return true;
    }

    size_t Socket::send(const std::string &_data)
    {
        send(_data.c_str(), _data.size());
    }

    size_t Socket::send(const uint8_t *_data, size_t _size)
    {
        return ::send(m_socket, _data, _size, );
    }

    const uint8_t *Socket::receive(int &_error, size_t _size)
    {
        uint8_t *data = nullptr;

        _error = recv(m_socket, data, _size);
        return data;
    }

    bool Socket::close()
    {
        ::close(m_fd);
    }

    Socket::Socket(int _proto)
        : m_proto(_proto)
    {
    }

    int Socket::create(int _dom, int _proto)
    {
        m_socket = socket(_dom, m_type, _proto);
    }

    int Socket::raw() const
    {
        return m_fd;
    }

    namespace tcp
    {
        Socket::Socket()
            : ::net::Socket(SOCK_STREAM)
        {
        }
    }

    namespace udp
    {
        Socket::Socket()
            : ::net::Socket(SOCK_DGRAM)
        {
        }
    }
}