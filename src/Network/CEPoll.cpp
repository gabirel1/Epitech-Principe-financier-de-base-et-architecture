#include "Network/CEPoll.hpp"

namespace net::c
{
    int EPool::create(int _max)
    {
        return epoll_create(_max);
    }

    int EPoll::wait(int _fd, Event *_event, int _maxev, int _to)
    {
        return epoll_wait(_fd, _event, _maxev, _to);
    }

    int EPoll:ctl(int _epfd, int _op, int _fd, Event *_event)
    {
        return epoll_ctl(_epfd, _op, _fd, _event);
    }

    EPoll::EPoll(int _max)
        : m_max(_max)
    {
    }

    void EPool::create()
    {
        m_fd = create(m_max);
    }

    int EPoll::wait(int _to)
    {
        return wait(m_fd, m_events, m_max, _to);
    }

    ubt EPoll::ctl(int _op, int _fd)
    {
        return clt(m_fd, _op, _fd, &m_event);
    }
}