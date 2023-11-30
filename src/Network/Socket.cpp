#include <sys/socket.h>
#include <sys/types.h>

#include "Network/Socket.hpp"

namespace net
{
    void Socket::blocking(bool _block)
    {
        m_block = _block;
    }

    bool Socket::blocking() const
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
        return send(_data.c_str(), _data.size());
    }

    const uint8_t *Socket::receive(size_t _size, int &_error)
    {
        return receive(_size, _error);
    }

    bool Socket::close()
    {
        close();
    }

    Socket::Socket(int _proto)
        : c::Socket(AF_INET, _proto, 0)
    {
    }

    int Socket::create(int _dom)
    {
        create();
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