#include "Core/Pipeline/InNetwork.hpp"

namespace pip
{
    template<IsSocket T>
    InNetwork<T>::InNetwork(std::vector<typename InNetwork<T>::Client> &_clients, NetToSerial &_output, uint32_t _port)
        : m_clients(_clients), m_output(_output), m_acceptor(), m_selector()
    {
        m_acceptor.listen(_port);
    }

    template<IsSocket T>
    InNetwork<T>::~InNetwork()
    {
        PipeType::stop();
    }

    template<IsSocket T>
    bool InNetwork<T>::start()
    {
        if (!m_running) {
            m_running = true;
            PipeType::tstart(this);
        }
        return m_running;
    }

    template<IsSocket T>
    void InNetwork<T>::status(float _to)
    {
        PipeType::tstatus(static_cast<ms>(_to * 1000));
    }

    template<IsSocket T>
    void InNetwork<T>::loop()
    {
        Client accept = nullptr;
        std::vector<Client> clients;

        while (m_running) {
            accept = m_acceptor.accept();
            if (accept)
                m_clients.push_back(accept);
            clients = m_selector.pull();
            for (Client &_client : clients)
                process(_client);
        }
    }

    template<IsSocket T>
    void InNetwork<T>::process(typename InNetwork<T>::Client _client)
    {
        int error = 0;

        m_output.push(_client->receive(NET_RECV_SIZE, error));
    }
}