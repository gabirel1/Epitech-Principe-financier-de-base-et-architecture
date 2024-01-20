#include <future>

#include "Server/Core/Pipeline/InNetwork.hpp"
#include "Common/Core/Logger.hpp"
#include "Common/Message/Message.hpp"

namespace pip
{
    template<IsSocket T, auto _T, class __T>
    requires SocketClient<__T, T>
    InNetwork<T, _T, __T>::InNetwork(std::vector<__T> &_clients, NetToAction &_output, RawOutput &_error, uint32_t _port)
        : m_clients(_clients), m_output(_output), m_error(_error), m_acceptor(), m_selector()
    {
        (void)m_acceptor.listen(_port);
        (void)m_acceptor.blocking(false);
        m_selector.timeout(100);
        Logger::Log("[InNetwork] listening to port: ", _port);
    }

    template<IsSocket T, auto _T, class __T>
    requires SocketClient<__T, T>
    InNetwork<T, _T, __T>::~InNetwork()
    {
        (void)this->template stop();
    }

    template<IsSocket T, auto _T, class __T>
    requires SocketClient<__T, T>
    bool InNetwork<T, _T, __T>::start()
    {
        if (!this->m_running)
            this->template tstart(this);
        Logger::Log("[InNetwork] Running: ", this->m_running);
        return this->m_running;
    }

    template<IsSocket T, auto _T, class __T>
    requires SocketClient<__T, T>
    void InNetwork<T, _T, __T>::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "Network Input");

        Client accept = nullptr;
        std::vector<Client> clients;

        while (this->m_running) {
            accept = m_acceptor.accept();
            if (accept) {
                m_clients.emplace_back(accept);
                m_selector.client(accept);
                Logger::Log("[InNetwork] Accepted new client: "); // todo log
            }
            clients = m_selector.pull();
            if (clients.size())
                Logger::Log("[InNetwork] Received event from: ", clients.size(), " clients");
            for (Client &_client : clients) {
                auto client = std::find_if(m_clients.begin(), m_clients.end(), [_client] (const ClientSocket _lclient) {
                    return _client == _lclient.getSocket();
                });
                if (client == m_clients.end()) {
                    fix::Reject reject;

                    (void)_client->close();
                    Logger::Log("[InNetwork] Unable to find the client's information: "); // todo log
                    // build reject
                    m_error.push(ErrorMsg(ClientSocket(_client), reject));
                    continue;
                } else if (_T(*client, m_output, m_error)) {
                    Logger::Log("[InNetwork] Disconnecting client: "); // todo log
                    m_clients.erase(client);
                    m_selector.erase(client->getSocket());
                }
            }
        }
    }
}