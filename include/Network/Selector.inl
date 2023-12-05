#include <algorithm>

#include "Network/Selector.hpp"

namespace net
{
    template<IsSocket T>
    Selector<T>::Selector()
        : c::EPoll(MAX_EVENT_EPOLL)
    {
    }

    template<IsSocket T>
    bool Selector<T>::client(Client _client)
    {
        if (clt(EPOLL_CTL_ADD, _client.raw()) != -1) {
            m_clients.emplace(_client.raw(), _client);
            return true;
        }
        return false;
    }

    template<IsSocket T>
    void Selector<T>::timeout(float _to)
    {
        m_to = _to;
    }

    template<IsSocket T>
    float Selector<T>::timeout() const
    {
        return m_to;
    }

    template<IsSocket T>
    std::vector<typename Selector<T>::Client> Selector<T>::pull()
    {
        Event *events;
        size_t set = wait(events, m_to);
        std::vector<Client> clients{set};

        for (size_t it = 0; it < std::min(set, MAX_SOCKET); it++)
            clients.emplace_back(m_clients.at(events[it].data.fd));
        return clients;
    }
}