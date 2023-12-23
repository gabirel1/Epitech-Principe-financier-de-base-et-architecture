#include <algorithm>

#include "Common/Network/Selector.hpp"

namespace net
{
    template<IsSocket T>
    Selector<T>::Selector()
        : c::EPoll((int)MAX_EVENT_EPOLL)
    {
        Logger::Log("[Selector] New selector with maximum of event from epoll: ", MAX_EVENT_EPOLL);
    }

    template<IsSocket T>
    bool Selector<T>::client(Client _client)
    {
        Event event;

        event.data.fd = _client->raw();
        event.events = EPOLLIN | EPOLLET;
        if (ctl(EPOLL_CTL_ADD, _client->raw(), &event) != -1) {
            m_clients.emplace(_client->raw(), _client);
            return true;
        }
        return false;
    }

    template<IsSocket T>
    void Selector<T>::erase(Client _client)
    {
        std::erase_if(m_clients, [_client] (const std::pair<int, Client> &_lclient) {
            return _client == _lclient.second;
        });
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
        Event *events = nullptr;
        size_t set = wait(events, m_to);
        std::vector<Client> clients{set};

        for (size_t it = 0; it < std::min(set, (size_t)MAX_SOCKET); it++)
            clients.emplace_back(m_clients.at(events[it].data.fd));
        return clients;
    }
}