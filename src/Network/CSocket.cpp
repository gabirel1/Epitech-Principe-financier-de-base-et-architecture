#include <cstddef>

#include <fcntl.h>

#include "Core/Logger.hpp"
#include "Network/CSocket.hpp"

namespace net::c
{
    Socket::Socket(int _dom, int _type, int _proto)
        : m_dom(_dom), m_type(_type), m_proto(_proto)
    {
        create();
    }

    Socket::~Socket()
    {
        close();
    }

    int Socket::create(int _dom, int _type, int _proto)
    {
        return socket(_dom, _type, _proto);
    }

    void Socket::bind(int _fd, struct sockaddr *_addr)
    {
        ::bind(_fd, _addr, sizeof(_addr));
    }

    void Socket::listen(int _fd, int _max)
    {
        ::listen(_fd, _max);
    }

    bool Socket::connect(int _fd, struct sockaddr *_addrs)
    {
        if (::connect(_fd, _addrs, sizeof(_addrs)) < 0)
            return false;
        return true;
    }

    int Socket::accept(int _fd)
    {
        return accept(_fd);
    }

    size_t Socket::send(int _fd, const uint8_t *_data, size_t _size)
    {
        return send(_fd, _data, _size);
    }

    const uint8_t *Socket::receive(int _fd, size_t _size, int &_error)
    {
        uint8_t *data = nullptr;

        _error = ::recv(_fd, data, _size, 0);
        return data;
    }

    bool Socket::blocking(int _fd, bool _block)
    {
        int flags = fcntl(_fd, F_GETFL, 0);

        if (flags == -1)
            return false;
        flags = _block ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
        return fcntl(_fd, F_SETFL, flags) == 0;
    }

    bool Socket::blocking(int _fd)
    {
        int flags = fcntl(_fd, F_GETFL, 0);

        return flags & O_NONBLOCK;
    }

    bool Socket::close(int _fd)
    {
        return close(_fd);
    }

    // non static function:

    void Socket::create()
    {
        Logger::Log("[c::Socket] Create a new socket with: domain(", m_dom, "), type(", m_type, ") protocol(", m_proto, ")");
        m_fd = create(m_dom, m_type, m_proto);
    }

    void Socket::bind(struct sockaddr *_addr)
    {
        Logger::Log("[c::Socket] Bind socket to: "); // todo log
        bind(m_fd, _addr);
    }

    void Socket::listen(int _max)
    {
        Logger::Log("[c::Socket] Initialisation of listening, with the maximum socket handling: ", _max);
        listen(m_fd, _max);
    }

    bool Socket::connect(struct sockaddr *_addr)
    {
        Logger::Log("[c::Socket] Connect to: "); // todo log
        if (::connect(m_fd, _addr, sizeof(_addr)) < 0) {
            Logger::Log("[c::Socket] Connection failed");
            return false;
        }
        Logger::Log("[c::Socket] Connection succed");
        return true;
    }

    int Socket::accept()
    {
        Logger::Log("[c::Socket] Accept new client");
        return ::accept(m_fd, (struct sockaddr *)NULL, NULL);
    }

    size_t Socket::send(const uint8_t *_data, size_t _size)
    {
        Logger::Log("[c::Socket] Send data of size: ", _size);
        return send(m_fd, _data, _size);
    }

    const uint8_t *Socket::receive(size_t _size, int &_error)
    {
        Logger::Log("[c::Socket] Receive data of maximum size: ", _size);
        return receive(m_fd, _size, _error);
    }

    bool Socket::blocking(bool _block)
    {
        Logger::Log("[c::Socket] Set blocking state to: ", _block);
        return blocking(m_fd, _block);
    }

    bool Socket::blocking() const
    {
        return blocking();
    }

    bool Socket::close()
    {
        Logger::Log("[c::Socket] Close");
        return close(m_fd);
    }

    void Socket::raw(int _fd)
    {
        m_fd = _fd;
    }

    int Socket::raw() const
    {
        return m_fd;
    }
}
