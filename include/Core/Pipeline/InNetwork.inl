#include "Core/Pipeline/InNetwork.hpp"
#include "Core/Logger.hpp"

namespace pip
{
    template<IsSocket T>
    InNetwork<T>::InNetwork(std::vector<typename InNetwork<T>::Client> &_clients, NetToSerial &_output, uint32_t _port)
        : m_clients(_clients), m_output(_output), m_acceptor(), m_selector()
    {
        m_acceptor.listen(_port);
        Logger::Log("[InNetwork] listening to port: ", _port);
    }

    template<IsSocket T>
    InNetwork<T>::~InNetwork()
    {
        this->template stop();
    }

    template<IsSocket T>
    bool InNetwork<T>::start()
    {
        if (!this->m_running)
            this->template tstart(this);
        Logger::Log("[InNetwork] Running: ", this->m_running);
        return this->m_running;
    }

    template<IsSocket T>
    void InNetwork<T>::status(float _to)
    {
        PipeType::tstatus(static_cast<ms>(_to * 1000));
    }

    template<IsSocket T>
    void InNetwork<T>::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "Network Input");

        Client accept = nullptr;
        std::vector<Client> clients;

        while (this->m_running) {
            accept = m_acceptor.accept();
            if (accept) {
                m_clients.push_back(accept);
                Logger::Log("[InNetwork] Accepted new client: "); // todo log
            }
            clients = m_selector.pull();
            for (Client &_client : clients)
                process(_client);
        }
    }

    template<IsSocket T>
    void InNetwork<T>::process(typename InNetwork<T>::Client _client)
    {
        int error = 0;
        Logger::Log("[InNetwork] Porcessing request from the client: "); // todo log

        // m_output.push(_client->receive(NET_RECV_SIZE, error));
    }
}