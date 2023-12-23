#include <future>

#include "Common/Core/Logger.hpp"
#include "Common/Message/Message.hpp"
#include "Common/Message/Utils.hpp"
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
        (void)stop();
    }

    bool Action::start()
    {
        if (!m_running)
            tstart(this);
        Logger::Log("[Action] Running: ", m_running);
        return m_running;
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
                // if (!input.Message.contains("35") || input.Message.at("35").size() != 1) {
                //     // send error to the error pipeline
                //     continue;
                // }
                // checking sum and header
                switch (input.Message.at("35")[0])
                {
                    case 'A': (void)treatLogon(input);
                        break;
                    case '0':
                        // Received --> Heartbeat (35=0) --> Heartbeat (user is still connected)
                        // Reply --> Heartbeat (35=0)
                        break;
                    case 'D': (void)treatNewOrderSingle(input);
                        break;
                    case 'F':
                        // Received --> Order Cancel Request (35=F) --> Order Cancel Request (user sent an order cancel, the order should be fully canceled)
                        // Reply --> Execution Report (35=8) or Order Cancel Reject (35=9)
                        break;
                    case 'G':
                        // Received --> Order Cancel/Replace Request (35=G) -> Order Cancel/Replace Request (user sent an order cancel, the order should be partially canceled, [meaning that the order quantity should be updated or the price should be changed])
                        // Reply --> Execution Report (35=8)
                        break;
                    case '5': (void)treatLogout(input);
                        break;
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

    bool Action::treatLogon(SerialIn &_input)
    {
        fix::Logon logon;
        fix::Reject reject;

        if (!_input.Message.contains("98") && !_input.Message.contains("108")) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        } else if (_input.Message.at("98") != "0") {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        }else if (!utils::is_numeric(_input.Message.at("108"))) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        } /*else if (!utils::to<UserId>(_input.Message.at("108")) != ) {
            // reject
            return false;
        }*/
        _input.Client.Logged = true;
        _input.Client.User = utils::to<UserId>(_input.Message.at("49"));
        _input.Client.SeqNumber = utils::to<size_t>(_input.Message.at("108"));
        logon.header.set49_SenderCompId(_input.Message.at("56")); // add to header
        logon.header.set56_TargetCompId(_input.Message.at("49")); // add to header
        logon.header.setSeqNum(_input.Message.at("34"));
        logon.set98_EncryptMethod("0");
        logon.set108_HeartBtInt(_input.Message.at("108"));
        m_raw.push({ _input.Client, logon });
        return true;
    }

    bool Action::treatLogout(SerialIn &_input)
    {
        fix::Logout logout;
        fix::Reject reject;

        if (_input.Client.Logged) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        }
        _input.Client.Logged = false;
        _input.Client.Disconnect = true;
        _input.Client.User = 0;
        logout.header.set49_SenderCompId(_input.Message.at("56")); // add to header
        logout.header.set56_TargetCompId(_input.Message.at("49")); // add to header
        logout.header.setSeqNum(std::to_string(_input.Client.SeqNumber));
        m_raw.push({ _input.Client, logout });
        return true;
    }

    bool Action::treatNewOrderSingle(SerialIn &_input)
    {
        SerialOut data;
        fix::Reject reject;

        if (!_input.Message.contains("11") || !_input.Message.contains("21")
            || !_input.Message.contains("55") || !_input.Message.contains("38")
            || !_input.Message.contains("44")) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        } else if (_input.Message.at("21") != "3" || (_input.Message.at("21") != "3"
            && _input.Message.at("21") != "4")) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        } else if (!utils::is_double(_input.Message.at("21")) || utils::is_double(_input.Message.at("48"))) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        }

        data.Client = _input.Client;
        data.OrderData.action = OrderBook::Data::Action::Add;
        data.OrderData.type = (_input.Message.at("21") == "3") ? OrderType::Ask : OrderType::Bid;
        data.OrderData.price = utils::to<Price>(_input.Message.at("48"));
        data.OrderData.order.userId = _input.Client.User;
        data.OrderData.order.orderId = utils::to<Quantity>(_input.Message.at("11"));
        data.OrderData.order.quantity = utils::to<OrderId>(_input.Message.at("11"));
        m_output.push(data);
        return true;
    }
}