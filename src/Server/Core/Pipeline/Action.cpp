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

    void Action::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "Action convertion");
        std::pair<bool, fix::Reject> reject;
        SerialIn input;

        while (m_running)
        {
            if (!m_input.empty())
            {
                input = m_input.pop_front();
                // need more checking, more info inside the function
                reject = fix::Header::Verify(input.Message);
                if (reject.first) {
                    reject.second.header.set49_SenderCompId(std::to_string(input.Client.User));
                    reject.second.header.set56_TargetCompId(input.Message.at("49"));
                    m_raw.push({ input.Client, reject.second });
                    continue;
                }
                switch (input.Message.at("35")[0])
                {
                    case 'A': (void)treatLogon(input);
                        break;
                    case '0': (void)treatHeartbeat(input);
                        break;
                    case 'D': (void)treatNewOrderSingle(input);
                        break;
                    case 'E': (void)treatNewOrderList(input);
                        break;
                    case 'F': (void)treatOrderCancelRequest(input);
                        break;
                    case 'G': (void)treatOrderCancelReplaceRequest(input);
                        break;
                    case '5': (void)treatLogout(input);
                        break;
                    case 'V': // Maybe not here (should be used in UDP server) --> Ask for market data
                        // Received --> Market Data Request (35=V)
                        // Reply --> Market Data Request Reject (35=Y) or Market Data Snapshot/Full Refresh (35=W)
                        break;
                    default: (void)treatUnknown(input);
                        break;
                }
            }
        }
    }

    bool Action::treatLogon(SerialIn &_input)
    {
        fix::Logon logon;
        std::pair<bool, fix::Reject> verif = fix::Logon::Verify(_input.Message);

        if (verif.first) {
            verif.second.set45_RefSeqNum(_input.Message.at("34"));
            m_raw.push({ _input.Client, verif.second });
            return false;
        }
        _input.Client.Logged = true;
        _input.Client.User = utils::to<UserId>(_input.Message.at("49"));
        _input.Client.SeqNumber = utils::to<size_t>(_input.Message.at("108"));
        logon.header.set49_SenderCompId(_input.Message.at("56"));
        logon.header.set56_TargetCompId(_input.Message.at("49"));
        logon.header.set34_msgSeqNum(_input.Message.at("34"));
        logon.set98_EncryptMethod("0");
        logon.set108_HeartBtInt(_input.Message.at("108"));
        m_raw.push({ _input.Client, logon });
        return true;
    }

    bool Action::treatLogout(SerialIn &_input)
    {
        fix::Logout logout;
        std::pair<bool, fix::Reject> reject = fix::Logout::Verify(_input.Message);

        if (_input.Client.Logged) {
            reject.second.set45_RefSeqNum(_input.Message.at("34"));
            reject.second.set58_Text("Client not connected");
            m_raw.push({ _input.Client, reject.second });
            return false;
        }
        _input.Client.Logged = false;
        _input.Client.Disconnect = true;
        _input.Client.User = 0;
        logout.header.set49_SenderCompId(_input.Message.at("56"));
        logout.header.set56_TargetCompId(_input.Message.at("49"));
        logout.header.set34_msgSeqNum(std::to_string(_input.Client.SeqNumber));
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
        data.OrderData.order.orderId = utils::to<OrderId>(_input.Message.at("11"));
        data.OrderData.order.quantity = utils::to<Quantity>(_input.Message.at("38"));
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

        if (!_input.Message.contains("11") || !_input.Message.contains("41")) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        } else if (_input.Message.at("54") != "3" && _input.Message.at("54") != "4") {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        }

        data.Client = _input.Client;
        data.OrderData.action = OrderBook::Data::Action::Cancel;
        data.OrderData.order.orderId = utils::to<OrderId>(_input.Message.at("41"));
        data.OrderData.type = (_input.Message.at("54") == "3") ? OrderType::Bid : OrderType::Ask;

        data.Time = _input.Time;
        m_output.push(data);
        return true;
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

        if (!_input.Message.contains("11") || !_input.Message.contains("41")
            || !_input.Message.contains("38") || !_input.Message.contains("44")
            || !_input.Message.contains("54") || !_input.Message.contains("40")) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        } else if ((_input.Message.at("54") != "3" && _input.Message.at("54") != "4")
            || _input.Message.at("40") != "2") {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        } else if (!utils::is_double(_input.Message.at("38")) || !utils::is_double(_input.Message.at("44"))) {
            // reject
            m_raw.push({ _input.Client, reject });
            return false;
        }

        data.Client = _input.Client;
        data.OrderData.action = OrderBook::Data::Action::Modify;
        data.OrderData.order.orderId = utils::to<OrderId>(_input.Message.at("41"));
        data.OrderData.order.quantity = utils::to<Quantity>(_input.Message.at("38"));
        data.OrderData.price = utils::to<Price>(_input.Message.at("44"));
        data.OrderData.type = (_input.Message.at("54") == "3") ? OrderType::Bid : OrderType::Ask;
        data.Time = _input.Time;
        return true;
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

        heartbeat.header.set49_SenderCompId(_input.Message.at("56"));
        heartbeat.header.set56_TargetCompId(_input.Message.at("49"));
        heartbeat.header.set34_msgSeqNum(_input.Message.at("34"));
        m_raw.push({ _input.Client, heartbeat });
        return true;
    }

}
