#include "Server/Core/Pipeline/InNetwork.hpp"
#include "Common/Core/Logger.hpp"
#include "Common/Message/Message.hpp"

namespace pip
{
    template<IsSocket T>
    InNetwork<T>::InNetwork(std::vector<ClientSocket> &_clients, NetToSerial &_output, RawOutput &_error, uint32_t _port)
        : m_clients(_clients), m_output(_output), m_error(_error), m_acceptor(), m_selector()
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
        int error = 0;

        while (this->m_running) {
            accept = m_acceptor.accept();
            if (accept) {
                m_clients.emplace_back(accept);
                m_selector.client(accept);
                Logger::Log("[InNetwork] Accepted new client: "); // todo log
            }
            clients = m_selector.pull();
            for (Client &_client : clients) {
                auto client = std::find_if(m_clients.begin(), m_clients.end(), [_client] (const ClientSocket _lclient) {
                    return _client == _lclient;
                });
                if (client == m_clients.end()) {
                    fix::Reject reject;

                    (void)_client->close();
                    Logger::Log("[InNetwork] Unable to find the client's information: "); // todo log
                    // build reject
                    m_error.push(ErrorMsg(ClientSocket(_client), reject));
                    continue;
                }
                process(*client);
            }
        }
    }

    template<IsSocket T>
    void InNetwork<T>::process(ClientSocket _client)
    {
        int error = 0;
        fix::Serializer::AnonMessage msg;
        fix::Reject reject; 

        if (!_client) {
            m_selector.erase(_client.getSocket());
            std::erase_if(m_clients, [_client] (const ClientSocket &_lclient) {
                return _client == _lclient;
            });
            Logger::Log("[InNetwork] Client disconnected: "); // todo log
            return;
        }
        std::string data(_client.getSocket()->receive(MAX_RECV_SIZE, error));

        if (error == 0) {
            Logger::Log("[InNetwork] Error: no data receive from the client: "); // todo log
            return;
        }
        if (fix::Serializer::run(data, msg) != fix::Serializer::Error::None) {
            Logger::Log("[InNetwork] Error: will parsing the client message: "); // todo log
            // build reject
            m_error.push(ErrorMsg(_client, reject));
            return;
        }
        Logger::Log("[InNetwork] Porcessing request from the client: "); // todo log
        m_output.push(NetOut(_client, msg));
    }
}