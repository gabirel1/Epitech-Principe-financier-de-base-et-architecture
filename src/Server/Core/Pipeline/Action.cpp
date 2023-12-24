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
                    case '0': (void)treatHeartbeat(input);
                        // Received --> Heartbeat (35=0) --> Heartbeat (user is still connected)
                        // Reply --> Heartbeat (35=0)
                        break;
                    case 'D': (void)treatNewOrderSingle(input);
                        break;
                    case 'E': (void)treatNewOrderList(input);
                        // Received --> New Order List (35=E) --> New Order List (user sent a list of orders)
                        // Reply --> Execution Report (35=8) or Order Cancel Reject (35=9)
                        break;
                    case 'F': (void)treatOrderCancelRequest(input);
                        // Received --> Order Cancel Request (35=F) --> Order Cancel Request (user sent an order cancel, the order should be fully canceled)
                        // Reply --> Execution Report (35=8) or Order Cancel Reject (35=9)
                        break;
                    case 'G': (void)treatOrderCancelReplaceRequest(input);
                        // Received --> Order Cancel/Replace Request (35=G) -> Order Cancel/Replace Request (user sent an order cancel, the order should be partially canceled, [meaning that the order quantity should be updated or the price should be changed])
                        // Reply --> Execution Report (35=8)
                        break;
                    case '5': (void)treatLogout(input);
                        break;
                    case 'V': // Maybe not here (should be used in UDP server) --> Ask for market data
                        // Received --> Market Data Request (35=V)
                        // Reply --> Market Data Request Reject (35=Y) or Market Data Snapshot/Full Refresh (35=W)
                        break;
                    default: (void)treatUnknown(input);
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

        reject.set45_RefSeqNum(_input.Message.at("34"));

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

        reject.set45_RefSeqNum(_input.Message.at("34"));

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

    bool Action::treatNewOrderList(SerialIn &_input)
    {
        SerialOut data;
        fix::Reject reject;

        reject.set45_RefSeqNum(_input.Message.at("34"));
        reject.set58_Text("Not implemented yet");
        // NOT IMPLEMENTED YET
        m_raw.push({ _input.Client, reject });
        return false;

        // if (!_input.Message.contains("66") || !_input.Message.contains("68")
        //     || !_input.Message.contains("73") || !_input.Message.contains("66")
        //     || !_input.Message.contains("11") || !_input.Message.contains("67")
        //     || !_input.Message.contains("54")) {
        //     // reject
        //     m_raw.push({ _input.Client, reject });
        // }
        // data.Client = _input.Client;

    }

    bool Action::treatNewOrderSingle(SerialIn &_input)
    {
        SerialOut data;
        fix::Reject reject;

        reject.set45_RefSeqNum(_input.Message.at("34"));

        if (!_input.Message.contains("11") || !_input.Message.contains("21")
            || !_input.Message.contains("55") || !_input.Message.contains("38")
            || !_input.Message.contains("44") || !_input.Message.contains("40")
            || !_input.Message.contains("54") || !_input.Message.contains("60")) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        } else if (_input.Message.at("21") != "3" || (_input.Message.at("54") != "3"
            && _input.Message.at("54") != "4") || _input.Message.at("40") != "2") {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        } else if (!utils::is_double(_input.Message.at("38")) || !utils::is_double(_input.Message.at("44"))) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        }

        data.Client = _input.Client;
        data.OrderData.action = OrderBook::Data::Action::Add;
        data.OrderData.type = (_input.Message.at("54") == "3") ? OrderType::Bid : OrderType::Ask;
        data.OrderData.price = utils::to<Price>(_input.Message.at("44"));
        data.OrderData.order.userId = _input.Client.User;
        data.OrderData.order.orderId = utils::to<Quantity>(_input.Message.at("11"));
        data.OrderData.order.quantity = utils::to<OrderId>(_input.Message.at("38"));
        data.Time = _input.Time;
        m_output.push(data);
        return true;
    }

    bool Action::treatOrderCancelRequest(SerialIn &_input)
    {
        SerialOut data;
        fix::Reject reject;

        reject.set45_RefSeqNum(_input.Message.at("34"));
        reject.set58_Text("Not implemented yet");
        // NOT IMPLEMENTED YET
        m_raw.push({ _input.Client, reject });
        return false;
    }

    bool Action::treatOrderCancelReplaceRequest(SerialIn &_input)
    {
        SerialOut data;
        fix::Reject reject;

        reject.set45_RefSeqNum(_input.Message.at("34"));
        reject.set58_Text("Not implemented yet");
        // NOT IMPLEMENTED YET
        m_raw.push({ _input.Client, reject });
        return false;
    }

    bool Action::treatUnknown(SerialIn &_input)
    {
        fix::Reject reject;

        reject.set45_RefSeqNum(_input.Message.at("34"));
        reject.set58_Text("Unknown message type");

        m_raw.push({ _input.Client, reject });
        return false;
    }

    bool Action::treatHeartbeat(SerialIn &_input)
    {
        fix::HeartBeat heartbeat;

        heartbeat.header.set49_SenderCompId(_input.Message.at("56")); // add to header
        heartbeat.header.set56_TargetCompId(_input.Message.at("49")); // add to header
        heartbeat.header.setSeqNum(_input.Message.at("34"));
        m_raw.push({ _input.Client, heartbeat });
        return true;
    }

}
