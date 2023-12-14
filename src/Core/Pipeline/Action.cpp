#include "Core/Pipeline/Action.hpp"

namespace pip
{
    Action::Action(NetToSerial &_input, SerialToMarket &_output, RawOutput _raw)
        : m_input(_input), m_output(_output), m_raw(_raw)
    {
    }

    Action::~Action()
    {
        stop();
    }

    bool Action::start()
    {
        if (!m_running)
            tstart(this);
        Logger::Log("[InNetwork] Running: ", m_running);
        return m_running;
    }

    void Action::status(float _to)
    {
        PipeType::tstatus(static_cast<ms>(_to * 1000));
    }

    /// @brief Process all incoming raw message to make action
    void Action::loop()
    {
        SerialIn input;

        while (m_running) {
            if (!m_input.empty()) {
                input = m_input.pop_front();
                // processing of the message
            }
        }
    }
}