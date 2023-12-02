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
            m_client.emplace(_client.raw(), _client);
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
    std::vector<Selector<T>::Client> Selector<T>::pull()
    {
        Event *_event;
        size_t set = wait(m_to, _event);
        Client client = nullptr;

        for (size_t it = 0; it < std::min(set, MAX_SOCKET); it++)
            clients.emplace_back(m_clients.at(_event[it].data.fd));
        return clients;
    }
}