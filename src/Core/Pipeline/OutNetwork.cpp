#include "Core/Pipeline/OutNetwork.hpp"
#include "Core/Logger.hpp"

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
        std::string data;

        while (m_running) {
            if (!m_input.empty()) {
                NetIn input(std::move(m_input.pop_front()));

                Logger::Log("[OutNetwork] Sending data to: "); // todo
                data = static_cast<std::string>(input.Message); 
                input.Client.getSocket()->send(reinterpret_cast<const uint8_t *>(data.c_str()), data.size());
            }
        }
    }
}