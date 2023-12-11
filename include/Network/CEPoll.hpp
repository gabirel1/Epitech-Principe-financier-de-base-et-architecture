#pragma once

#include <sys/epoll.h>

namespace net::c
{
    class EPoll
    {

        public:
            using Event = struct epoll_event;

            EPoll() = default;
            EPoll(int _max);
            virtual ~EPoll() = default;

            [[nodiscard]] static int create(int _max);
            [[nodiscard]] static int wait(int _fd, Event *_events, int _maxev, int _to);
            [[nodiscard]] static int ctl(int _epfd, int _op, int _fd, Event *_event);

        protected:

            void create();
            [[nodiscard]] int wait(Event *_events, int _to);
            [[nodiscard]] int ctl(int _op, int _fd, Event *_event);

        private:
            int m_max = 0;

            int m_fd = 0;

            Event m_event;
    };
}