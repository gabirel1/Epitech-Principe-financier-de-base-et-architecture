#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "Core/Logger.hpp"
#include "Network/Socket.hpp"

namespace net
{
    void Socket::blocking(bool _block)
    {
        blocking(_block);
    }

    bool Socket::blocking() const
    {
        return blocking();
    }

    bool Socket::connect(const Ip &_ip, uint32_t _port)
    {
        create();
        connect(_ip, _port);
        return true;
    }

    size_t Socket::send(const std::string &_data)
    {
        return send(reinterpret_cast<const uint8_t *>(_data.c_str()), _data.size());
    }

    size_t Socket::send(const uint8_t *_data, size_t _size)
    {
        return send(_data, _size);
    }

    std::string Socket::receive(size_t _size, int &_error)
    {
        const uint8_t *data = c::Socket::receive(_size, _error);
        std::string str = "";

        if (_error != -1)
            str.assign(data, data + _error);
        return str;
    }

    bool Socket::close()
    {
        return close();
    }

    Socket::Socket(int _type)
        : c::Socket(AF_INET, _type, 0)
    {
    }

    namespace tcp
    {
        Socket::Socket()
            : ::net::Socket(SOCK_STREAM)
        {
            Logger::Log("[tcp::Socket] New TCP socket");
        }
    }

    namespace udp
    {
        Socket::Socket()
            : ::net::Socket(SOCK_DGRAM)
        {
            Logger::Log("[udp::Socket] New UDP socket");
        }
    }
}