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

                m_tp.enqueue([this, _input = std::move(input)] () mutable {
                    _input.Message.header.set49_SenderCompId("Market");
                    std::vector<ClientSocket>::iterator it;

                    if (_input.Client.getSocket()) {
                        it = std::find_if(m_clients.begin(), m_clients.end(), [userId = _input.Client.User] (const ClientSocket &_client) {
                            return _client.User == userId;
                        });
                    } else {
                        it = std::find_if(m_clients.begin(), m_clients.end(), [userId = _input.Message.header.getTargetCompId()] (const ClientSocket &_client) {
                            return _client.User == userId;
                        });
                    }

                    if (it != m_clients.end()) {
                        _input.Message.header.set34_msgSeqNum(std::to_string(it->SeqNumber));
                        _input.Message.header.set56_TargetCompId(std::to_string(it->User));
                        std::string data = _input.Message.to_string();

                        if (it->getSocket()) {
                            it->getSocket()->send(reinterpret_cast<const uint8_t *>(data.c_str()), data.size());
                            it->Logged = _input.Client.Logged;
                            if (it->Disconnect) {
                                it->getSocket()->close();
                                m_clients.erase(it);
                            }
                            logTiming(it);
                        } else {
                            m_clients.erase(it);
                        }
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