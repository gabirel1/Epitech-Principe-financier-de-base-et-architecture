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

                // if message doesn't contain 35 tag, we don't process it
                if (!input.Message.contains("35") || input.Message.at("35").size() != 1) {
                    // send error to the error pipeline
                    continue;
                }
                switch (input.Message.at("35")[0])
                {
                    case 'A':
                        // Received --> Logon (35=A) --> Login (user can now send orders)
                        // Reply --> Logon (35=A)
                        break;
                    case '0':
                        // Received --> Heartbeat (35=0) --> Heartbeat (user is still connected)
                        // Reply --> Heartbeat (35=0)
                        break;
                    case 'D':
                        // Received --> New Order Single (35=D) --> New Order Single (user sent an order)
                        // Reply --> Execution Report (35=8)
                        break;
                    case 'F':
                        // Received --> Order Cancel Request (35=F) --> Order Cancel Request (user sent an order cancel, the order should be fully canceled)
                        // Reply --> Execution Report (35=8) or Order Cancel Reject (35=9)
                        break;
                    case 'G':
                        // Received --> Order Cancel/Replace Request (35=G) -> Order Cancel/Replace Request (user sent an order cancel, the order should be partially canceled, [meaning that the order quantity should be updated or the price should be changed])
                        // Reply --> Execution Report (35=8)
                        break;
                    case '5':
                        // Received --> Logout --> Logout (user is now disconnected)
                        // Reply --> Logout ?? Not sure
                    case 'V': // Maybe not here (should be used in UDP server) --> Ask for market data
                        // Received --> Market Data Request (35=V)
                        // Reply --> Market Data Request Reject (35=Y) or Market Data Snapshot/Full Refresh (35=W)
                        break;
                    default:
                        // Unknown message type
                        // send error to the error pipeline
                        break;
                }
            }
        }
    }
}