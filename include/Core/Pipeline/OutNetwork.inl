#include "Core/Pipeline/OutNetwork.hpp"
#include "Core/Logger.hpp"

namespace pip
{
    template<IsSocket T>
    OutNetwork<T>::OutNetwork(MarketToNet &_input)
        : m_input(_input)
    {
    }

    template<IsSocket T>
    OutNetwork<T>::~OutNetwork()
    {
        this->template stop();
    }

    template<IsSocket T>
    bool OutNetwork<T>::start()
    {
        if (!this->m_running)
            this->template tstart(this);
        Logger::Log("[OutNetwork] Running: ", this->m_running);
        return this->m_running;
    }

    template<IsSocket T>
    void OutNetwork<T>::status(float _to)
    {
        PipeType::tstatus(static_cast<ms>(_to * 1000));
    }

    template<IsSocket T>
    void OutNetwork<T>::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "Network Output");

        NetIn input;
        std::string data;

        while (this->m_running) {
            if (!m_input.empty()) {
                input = m_input.pop_front();
                data = static_cast<std::string>(input.Message); 
                input.Client.getSocket()->send(reinterpret_cast<const uint8_t *>(data.c_str()), data.size());
            }
        }
    }
}