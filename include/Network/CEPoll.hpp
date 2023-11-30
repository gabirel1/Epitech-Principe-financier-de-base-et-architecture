#pragma once

#include <sys/epoll.h>

namespace net::c
{
    class EPoll
    {
        using Event = struct epoll_event;

        public:
            [[nodiscard]] static int create(int _max = 0);
            [[nodiscard]] static int wait(int _fd, Event *_event, int _maxev, int _to);
            [[nodiscard]] static int ctl(int _epfd, int _op, int _fd, Event *_event);

        protected:
            EPoll(int _max);
            virtual ~EPoll() = default;

            void create();
            [[nodiscard]] int wait(int _to);
            [[nodiscard]] int ctl(int _op, int _fd, Event *_event);

        private:
            int m_max;

            int m_fd;

            Event m_event;
            Event *m_events;
    };
}