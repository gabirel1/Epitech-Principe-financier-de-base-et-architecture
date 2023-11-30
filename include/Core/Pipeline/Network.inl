#include "Core/Pipeline/Network.hpp"

namespace pip
{
    template<IsSocket T>
    Network<T>::Network(std::vector<Client> &_clients, NetToSerial &_output, uint32_t _port)
        : m_clients(_clients), m_output(_output)
    {
        listen(_port);
    }

    template<IsSocket T>
    void Network<T>::start()
    {
        if (!m_running) {
            m_running = true;
            tstart(this);
        }
        return m_running;
    }

    template<IsSocket T>
    void Network<T>::status(float _to)
    {
        tstatus(static_cast<ms>(_to * 1000));
    }

    template<IsSocket T>
    void Network<T>::loop()
    {
        Acceptor<T>::Client accept = nullptr;
        std::vector<Client> clients;

        while (m_running) {
            accept = m_acceptor.accept();
            if (accept)
                m_client.push_back(accept);
            clients = m_select.pull();
            for (Client &_client : clients)
                process(_client);
        }
    }

    template<IsSocket T>
    void Network<T>::process(Client &_client)
    {
        m_output.enqueue();
    }
}