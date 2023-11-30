#include "Network/Selector.hpp"

namespace net
{
    Selector::Selector()
        : c::EPoll(MAX_EVENT_EPOLL)
    {
    }

    bool Selector::client(Client _client)
    {
        if (clt(EPOLL_CTL_ADD, _client.raw()) != -1) {
            m_client.emplace(_client.raw(), _client);
            return true;
        }
        return false;
    }

    void Selector::timeout(float _to)
    {
        m_to = _to;
    }

    float Selector::timeout() const
    {
        return m_to;
    }

    std::vector<Client> pull()
    {
        int set = wait(m_to);
        Client client = nullptr;

        for (int n = 0; n < std::min(set, MAX_SOCKET); n++)
            clients.emplace_back(std::move(m_clients.at()));
        return clients;
    }
}