#pragma once

#include <map>
#include <memory>

#include "Common/Network/Acceptor.hpp"
#include "Common/Network/CEPoll.hpp"

#ifndef MAX_EVENT_EPOLL
    #define MAX_EVENT_EPOLL 30
#endif

#ifndef MAX_SOCKET
    #define MAX_SOCKET 30
#endif

namespace net
{
    template<IsSocket T>
    class Selector : public c::EPoll
    {
        public:
            using Client = Acceptor<T>::Client;

            Selector();
            ~Selector() = default;

            bool client(Client _client);
            void erase(Client _client);


            void timeout(float _to);
            [[nodiscard]] float timeout() const;

            std::vector<Client> pull();

            [[nodiscard]] size_t size() const;

        private:
            std::map<int, Client> m_clients;

            float m_to;
    };
}

#include "Common/Network/Selector.inl"