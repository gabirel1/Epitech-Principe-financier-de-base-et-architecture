#include "Network/CSocket.hpp"

namespace net::c
{
    Socket::Socket(int _dom, int _type, int _proto)
        : m_dom(_dom), m_type(_type), m_proto(_proto)
    {
    }

    Socket::~Socket()
    {
        void = close();
    }

    int Socket::create(int _dom, int _type, int _proto)
    {
        return ::create(_dom, _type, _proto);
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

    void Socket::accept(int _fd)
    {
        return accept(_fd, (struct sockaddr *)NULL, NULL);
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

    bool Socket::blocking(int _fd) const
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

    void Socket::create()
    {
        m_fd = create(m_dom, m_type, m_proto);
    }

    void Socket::bind(struct sockaddr *_addr)
    {
        bind(m_fd, _addr);
    }

    void Socket::listen(int _max)
    {
        listen(m_fd, _max);
    }

    bool Socket::connect(struct sockaddr *_addrs)
    {
        if (::connect(m_fd, (struct sockaddr *)_addrs, sizeof(_addrs)) < 0)
            return false;
        return true;
    }

    int Socket::accept()
    {
        return ::accept(m_fd, (struct sockaddr *)NULL, NULL);
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

    bool Socket::blocking() const
    {

    }

    bool Socket::close()
    {
        return close(m_fd);
    }

    void Socket::raw(int _fd)
    {
        m_fd = _fd;
    }

    int Socker::raw() const
    {
        return m_fd;
    }
}
