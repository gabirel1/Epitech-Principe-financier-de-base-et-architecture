#include "Network/CSocket.hpp"

namespace net::c
{
    bool Socket::connect(int _fd, struct sockaddr *_addrs)
    {
        if (::connect(_fd, (struct sockaddr *)_addrs, sizeof(_addrs)) < 0)
            return false;
        return true;
    }

    size_t Socket::send(int _fd, const uint8_t *_data, size_t _size)
    {
        return send(_fd, _data, _size);
    }

    const uint8_t *Socket::receive(int _fd, size_t _size, int &_error)
    {
        uint8_t *data = nullptr;

        _error = ::recv(_fd, data, _size);
        return data;
    }

    void Socket::blocking(int _fd, bool _block)
    {

    }

    bool Socket::isBlocking(int _fd) const
    {

    }

    bool Socket::close(int _fd)
    {
        return close(_fd);
    }

    int Socket::create(int _dom, int _type, int _proto)
    {
        return socket(_dom, _type, _proto);
    }

    // non static function:

    bool Socket::connect(struct sockaddr *_addrs)
    {
        if (::connect(m_fd, (struct sockaddr *)_addrs, sizeof(_addrs)) < 0)
            return false;
        return true;
    }

    size_t Socket::send(const uint8_t *_data, size_t _size)
    {
        return send(_fd, _data, _size);
    }

    const uint8_t *Socket::receive(size_t _size, int &_error)
    {
        uint8_t *data = nullptr;

        _error = receive(_fd, data, _size);
        return data;
    }

    void Socket::blocking(bool _block)
    {

    }

    bool Socket::isBlocking() const
    {

    }

    bool Socket::close()
    {
        return close(m_fd);
    }

    void Socket::create()
    {
        m_fd = create(m_dom, m_type, m_proto);
    }

    Socket::Socket(int _dom, int _type, int _proto)
        : m_dom(_dom), m_type(_type), m_proto(_proto)
    {
    }

    Socket::~Socket()
    {
        void = close();
    }
}
