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
                    std::vector<ClientSocket>::iterator it;
                    UserId userId = "";

                    _input.Message.header.set49_SenderCompId(PROVIDER_NAME);
                    Logger::Log("client list size: ", m_clients.size());
                    if (_input.Client.getSocket()) {
                        it = std::find_if(m_clients.begin(), m_clients.end(), [_socket = _input.Client.getSocket()] (const ClientSocket &_client) {
                            return _client.getSocket() == _socket;
                        });
                        userId = _input.Client.User;
                        Logger::Log("[OutNetwork] (Reply) Comming from an Action pipeline");
                    } else {
                        it = std::find_if(m_clients.begin(), m_clients.end(), [userId = _input.Client.User] (const ClientSocket &_client) {
                            return _client.User == userId;
                        });
                        Logger::Log("[OutNetwork] (Reply) Comming from an anonymous pipeline");
                    }

                    if (it != m_clients.end()) {
                        userId = it->User;
                        _input.Message.header.set34_msgSeqNum(std::to_string(it->SeqNumber));
                        _input.Message.header.set56_TargetCompId(userId);
                        std::string data = _input.Message.to_string();

                        if (it->getSocket()) {
                            it->getSocket()->send(reinterpret_cast<const uint8_t *>(data.c_str()), data.size());
                            Logger::Log("[OutNetwork] Data send successfuly: ", data);
                            it->Logged = _input.Client.Logged;
                            Logger::Log("[OutNetwork] New login status: ", it->Logged);
                            if (_input.Client.Disconnect) {
                                it->getSocket()->close();
                                m_clients.erase(it);
                                Logger::Log("[OutNetwork] Client has been disconnected: ", userId);
                            }
                            logTiming(it);
                        } else {
                            Logger::Log("[OutNetwork] Client not connected: ", userId);
                            m_clients.erase(it);
                        }
                    } else {
                        Logger::Log("[OutNetwork] Client not found: ", userId);
                    }
                });
            }
        }
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