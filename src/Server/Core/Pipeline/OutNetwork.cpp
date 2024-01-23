#include <future>

#include "Common/Core/Logger.hpp"
#include "Server/Core/Pipeline/OutNetwork.hpp"

namespace pip
{
    OutNetwork::OutNetwork(MarketToNet &_input, std::vector<ClientSocket> &_clients)
        : m_input(_input), m_clients(_clients)
    {
    }

    OutNetwork::~OutNetwork()
    {
        (void)stop();
    }

    bool OutNetwork::start()
    {
        if (!m_running)
            tstart(this);
        Logger::Log("[OutNetwork] Running: ", m_running);
        return m_running;
    }

    void OutNetwork::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "Network Output");
        NetIn input;

        while (m_running) {
            if (!m_input.empty()) {
                input = m_input.pop_front();

                Logger::Log("[OutNetwork] New message message to send: ", input.Message.to_string());
                m_tp.enqueue([this, _input = std::move(input)] () mutable {
                    auto [client, userId] = getClientInfo(_input);

                    _input.Message.header.set49_SenderCompId(PROVIDER_NAME);
                    if (client != m_clients.end()) {
                        _input.Message.header.set34_msgSeqNum(std::to_string((client->SeqNumber)++));
                        if (userId != "")
                            _input.Message.header.set56_TargetCompId(userId);
                        std::string data = _input.Message.to_string();

                        if (client->getSocket()) {
                            if (client->getSocket()->send(reinterpret_cast<const uint8_t *>(data.c_str()), data.size()) == data.size())
                                Logger::Log("[OutNetwork] Data send successfuly: ", data);
                            else
                                Logger::Log("[OutNetwork] Error occured when sending data");
                            client->Logged = _input.Client.Logged;
                            client->User = userId;
                            client->Disconnect = _input.Client.Disconnect;
                            logTiming(client);
                            Logger::Log("[OutNetwork] Updated client status: "); // todo log
                            if (_input.Client.Disconnect) {
                                client->getSocket()->close();
                                Logger::Log("[OutNetwork] Client has been disconnected: ", userId);
                            }
                        } else {
                            Logger::Log("[OutNetwork] Client not connected: ", userId);
                            m_clients.erase(client);
                        }
                    } else {
                        Logger::Log("[OutNetwork] Client not found: ", userId);
                    }
                });
            }
        }
    }

    OutNetwork::ClientInfo OutNetwork::getClientInfo(const NetIn &_input) const
    {
        std::vector<ClientSocket>::iterator it;
        UserId userId = "";

        if (_input.Client.getSocket()) {
            it = std::find_if(m_clients.begin(), m_clients.end(), [_socket = _input.Client.getSocket()] (const ClientSocket &_client) {
                return _client.getSocket() == _socket;
            });
            userId = _input.Client.User;
            Logger::Log("[OutNetwork] (Reply) Comming from an Action pipeline");
        } else {
            Logger::Log("Client give by the pipeline: ", _input.Client.User);
            it = std::find_if(m_clients.begin(), m_clients.end(), [_userId = _input.Client.User] (const ClientSocket &_client) {
                return _client.User == _userId;
            });
            if (it != m_clients.end())
                userId = it->User;
            Logger::Log("[OutNetwork] (Reply) Comming from an anonymous pipeline");
        }
        return ClientInfo(it, userId);
    }

    void OutNetwork::logTiming(std::vector<ClientSocket>::iterator _it)
    {
        if (!_it->hasRequest(_it->SeqNumber - 1))
            return;
        auto start = _it->getRequest(_it->SeqNumber - 1);
        auto end = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        Logger::Log("[OutNetwork] Send respond after: ", diff.count(), " ms");
    }
}