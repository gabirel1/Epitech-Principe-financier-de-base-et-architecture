#pragma once

#include <memory>

#include "Network/CEPoll.hpp"
#include "Network/Socket.hpp"

#ifndef MAX_EVENT_EPOLL
    #define MAX_EVENT_EPOLL
#endif

#ifndef MAX_SOCKET
    #define MAX_SOCKET 30
#endif

namespace net
{
    using Client = std::unique_ptr<Socket>;

    class Selector : public c::EPoll
    {
        public:
            Selector() = default;
            ~Selector() = default;

            void client(Client _client);

            void timeout(float _to);
            [[nodiscard]] float timeout() const;

            Client pull();

        private:
            std::map<int, Client> m_clients;

            float m_to;
    };
}