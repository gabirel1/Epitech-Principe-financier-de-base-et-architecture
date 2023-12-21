#include "Common/Core/Logger.hpp"
#include "Server/Core/Pipeline/Action.hpp"
#include "Server/Core/meta.hpp"

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

        SerialIn input;

        while (m_running)
        {
            if (!m_input.empty())
            {
                input = m_input.pop_front();
                // processing of the message

                // if message doesn't contain 35 tag, we don't process it
                if (input.Message.contains("35") == false || input.Message.at("35").size() == 0)
                    continue;
                char msg_type = input.Message.at("35")[0];
                switch (msg_type)
                {
                case 'A':
                    // Received --> Logon (35=A)
                    // Reply --> Logon (35=A)
                    break;
                case '0':
                    // Received --> Heartbeat (35=0)
                    // Reply --> Heartbeat (35=0)
                    break;
                case 'D':
                    // Received --> New Order Single (35=D)
                    // Reply --> Execution Report (35=8)
                    break;
                case 'F':
                    // Received --> Order Cancel Request (35=F)
                    // Reply --> Execution Report (35=8) or Order Cancel Reject (35=9)
                    break;
                case 'G':
                    // Received --> Order Cancel/Replace Request (35=G)
                    // Reply --> Execution Report (35=8)
                    break;
                case '5':
                    // Received --> Logout
                    // Reply --> Logout ?? Not sure
                case 'V': // Maybe not here (should be used in UDP server)
                    // Received --> Market Data Request (35=V)
                    // Reply --> Market Data Request Reject (35=Y) or Market Data Snapshot/Full Refresh (35=W)
                    break;
                default:
                    // Unknown message type
                    break;
                }
            }
        }
    }
}