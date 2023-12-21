#include "Common/Core/Logger.hpp"
#include "Server/Core/Pipeline/OutNetwork.hpp"

namespace pip
{
    OutNetwork::OutNetwork(MarketToNet &_input)
        : m_input(_input)
    {
    }

    OutNetwork::~OutNetwork()
    {
        stop();
    }

    bool OutNetwork::start()
    {
        if (!m_running)
            tstart(this);
        Logger::Log("[OutNetwork] Running: ", m_running);
        return m_running;
    }

    void OutNetwork::status(float _to)
    {
        PipeType::tstatus(static_cast<ms>(_to * 1000));
    }

    void OutNetwork::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "Network Output");

        NetIn input;

        while (m_running) {
            if (!m_input.empty()) {
                input = m_input.pop_front();

                m_tp.enqueue([_input = std::move(input)] () mutable {
                    std::string data = _input.Message.to_string();

                    _input.Client.getSocket()->send(reinterpret_cast<const uint8_t *>(data.c_str()), data.size());
                    Logger::Log("[OutNetwork] Send data to: ");
                });
            }
        }
    }
}