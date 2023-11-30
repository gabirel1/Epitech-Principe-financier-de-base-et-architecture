#include <sys/socket.h>
#include <sys/types.h>

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

        memset(&addrs, 0, sizeof(addrs));
        addrs.sin_family = AF_INET;
        addrs.sin_port = htons(_port);
        m_create.create(AF_INET, SOCK_STREAM, 0);
        m_socket.connect(&addrs);
        return true;
    }

    size_t Socket::send(const std::string &_data)
    {
        return send(_data.c_str(), _data.size());
    }

    size_t Socket::send(const uint8_t *_data, size_t _size)
    {
        return m_socket.send(_data.c_str(), _data.size());
    }

    const uint8_t *Socket::receive(size_t _size, int &_error)
    {
        return m_socket.receive(_size, _error);
    }

    bool Socket::close()
    {
        m_socket.close();
    }

    Socket::Socket(int _proto)
        : m_socket(AF_INET, _proto, 0)
    {
    }

    int Socket::create(int _dom)
    {
        m_socket.create();
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