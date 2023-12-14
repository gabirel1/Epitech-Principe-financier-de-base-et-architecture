#include "Core/Pipeline/Action.hpp"
#include "Core/Logger.hpp"
#include "Core/meta.hpp"

namespace pip
{
    Action::Action(NetToSerial &_input, SerialToMarket &_output, RawOutput &_raw)
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
        Logger::Log("[Action] Running: ", m_running);
        return m_running;
    }

    void Action::status(float _to)
    {
        PipeType::tstatus(static_cast<ms>(_to * 1000));
    }

    /// @brief Process all incoming raw message to make action
    void Action::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "Action convertion");


        while (m_running) {
            if (!m_input.empty()) {
                SerialIn input(std::move(m_input.pop_front()));
                // processing of the message
            }
        }
    }
}