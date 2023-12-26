#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "Common/Core/Logger.hpp"
#include "Common/Network/Socket.hpp"

namespace net
{
    void Socket::blocking(bool _block)
    {
        (void)c_blocking(_block);
    }

    bool Socket::connect(const Ip &_ip, uint32_t _port)
    {
        if (!c_create())
            return false;
        return c_connect(_ip.c_str(), _port);
    }

    size_t Socket::send(const std::string &_data)
    {
        return c_send(reinterpret_cast<const uint8_t *>(_data.c_str()), _data.size());
    }

    size_t Socket::send(const uint8_t *_data, size_t _size)
    {
        return c_send(_data, _size);
    }

    std::string Socket::receive(size_t _size, int &_error)
    {
        const uint8_t *data = c_receive(_size, _error);
        std::string str = "";

        if (_error == -1) {
            Logger::Log("[Socket] Error will trying to receive data: ", strerror(errno));
            if (data != nullptr)
                delete[] data;
            return str;
        }
        str.assign(data, data + _error);
        delete[] data;
        return str;
    }

    bool Socket::close()
    {
        return c_close();
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