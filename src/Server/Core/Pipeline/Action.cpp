#include <future>

#include "Common/Core/Logger.hpp"
#include "Common/Core/Utils.hpp"
#include "Common/Message/Message.hpp"
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
                    Logger::Log("[Action] Incorect header received from client: ", input.Client.User);
                    reject.second.header.set56_TargetCompId(std::to_string(input.Client.User));
                    reject.second.header.set49_SenderCompId(input.Message.at(fix::Tag::TargetCompId));  // WARN: if the tag is missing?
                    m_raw.push({ input.Client, reject.second });
                    continue;
                }
                Logger::Log("[Action] Received message from: ", input.Client.User, " with type: ", input.Message.at(fix::Tag::MsgType));
                switch (input.Message.at("35")[0])
                {
                    case fix::Logon::cMsgType: (void)treatLogon(input);
                        break;
                    case fix::HeartBeat::cMsgType: (void)treatHeartbeat(input);
                        break;
                    case fix::NewOrderSingle::cMsgType: (void)treatNewOrderSingle(input);
                        break;
                    /*case 'F': (void)treatOrderCancelRequest(input);
                        break;*/
                    case fix::OrderCancelReplaceRequest::cMsgType: (void)treatOrderCancelReplaceRequest(input);
                        break;
                    case fix::Logout::cMsgType: (void)treatLogout(input);
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

        Logger::Log("[Action] (Logon) Treating message from: ", _input.Client.User);
        if (verif.first) {
            Logger::Log("[Action] (Logon) Request verification failed: "); // todo log
            verif.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            m_raw.push({ _input.Client, verif.second });
            return false;
        }
        _input.Client.Logged = true;
        _input.Client.User = utils::to<UserId>(_input.Message.at(fix::Tag::SenderCompId));
        _input.Client.SeqNumber = utils::to<size_t>(_input.Message.at(fix::Tag::MsqSeqNum));
        logon.set98_EncryptMethod("0");
        logon.set108_HeartBtInt(_input.Message.at(fix::Tag::HearBtInt));
        Logger::Log("[Action] (Logon) Request from ", _input.Client.User, " sucessfuly handle: ");
        m_raw.push({ _input.Client, logon });
        return true;
    }

    bool Action::treatLogout(SerialIn &_input)
    {
        fix::Logout logout;
        std::pair<bool, fix::Reject> reject = fix::Logout::Verify(_input.Message);
        Logger::Log("[Action] (Logout) Treating message from: ", _input.Client.User);

        if (_input.Client.Logged) {
            Logger::Log("[Action] (Logout) Request verification failed: "); // todo log
            reject.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            reject.second.set58_text("Client not connected");
            m_raw.push({ _input.Client, reject.second });
            return false;
        }
        _input.Client.Logged = false;
        _input.Client.Disconnect = true;
        _input.Client.User = 0;
        Logger::Log("[Action] (Logout) Request from: ", _input.Client.User, ", sucessfuly handle");
        m_raw.push({ _input.Client, logout });
        return true;
    }

    bool Action::treatNewOrderSingle(SerialIn &_input)
    {
        SerialOut data;
        std::pair<bool, fix::Reject> verif = fix::NewOrderSingle::Verify(_input.Message);
        Logger::Log("[Action] (New Order Single) Treating message from: ", _input.Client.User);

        if (verif.first) {
            Logger::Log("[Action] (New Order Single) Request verification failed: "); // todo log
            verif.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            m_raw.push({ _input.Client, verif.second });
            return false;
        }
        data.Client = _input.Client;
        data.OrderData.action = OrderBook::Data::Action::Add;
        data.OrderData.type = (_input.Message.at(fix::Tag::Side) == "3") ? OrderType::Bid : OrderType::Ask;
        data.OrderData.price = utils::to<Price>(_input.Message.at(fix::Tag::Price));
        data.OrderData.order.userId = _input.Client.User;
        data.OrderData.order.orderId = utils::to<OrderId>(_input.Message.at(fix::Tag::ClOrdID));
        data.OrderData.order.quantity = utils::to<Quantity>(_input.Message.at(fix::Tag::OrderQty));
        Logger::Log("[Action] (New Order Single) Waiting for action from data: "); // todo log
        m_output.push(data);
        return true;
    }

    bool Action::treatOrderCancelRequest(SerialIn &_input)
    {
        SerialOut data;
        std::pair<bool, fix::Reject> verif = fix::OrderCancelRequest::Verify(_input.Message);

        if (verif.first) {
            m_raw.push({ _input.Client, verif.second });
            return false;
        }
        data.Client = _input.Client;
        data.OrderData.action = OrderBook::Data::Action::Cancel;
        data.OrderData.order.orderId = utils::to<OrderId>(_input.Message.at(fix::Tag::OrigClOrdID));
        data.OrderData.order.userId = _input.Client.User;
        data.OrderData.type = (_input.Message.at(fix::Tag::Side) == "3") ? OrderType::Bid : OrderType::Ask;
        m_output.push(data);
        return true;
    }

    bool Action::treatOrderCancelReplaceRequest(SerialIn &_input)
    {
        SerialOut data;
        std::pair<bool, fix::Reject> verif = fix::OrderCancelReplaceRequest::Verify(_input.Message);
        Logger::Log("[Action] (Order Cancel Replace) Treating message from: ", _input.Client.User);

        if (verif.first) {
            Logger::Log("[Action] (Order Cancel Replace) Request verification failed: "); // todo log
            verif.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            m_raw.push({ _input.Client, verif.second });
            return false;
        }
        data.Client = _input.Client;
        data.OrderData.action = OrderBook::Data::Action::Modify;
        data.OrderData.order.userId = _input.Client.User;
        data.OrderData.order.orderId = utils::to<OrderId>(_input.Message.at(fix::Tag::OrigClOrdID));
        data.OrderData.order.quantity = utils::to<Quantity>(_input.Message.at(fix::Tag::OrderQty));
        data.OrderData.price = utils::to<Price>(_input.Message.at(fix::Tag::Price));
        data.OrderData.type = (_input.Message.at(fix::Tag::Side) == "3") ? OrderType::Bid : OrderType::Ask;
        Logger::Log("[Action] (Order Cancel Replace) Waiting for action from data: "); // todo log
        m_output.push(data);
        return true;
    }

    bool Action::treatUnknown(SerialIn &_input)
    {
        fix::Reject reject;

        Logger::Log("[Action] (New Order Single) Rejecting request from client: ", _input.Client.User, ", with request type: ", _input.Message.at(fix::Tag::MsgType));
        reject.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
        reject.set371_refTagId(fix::Tag::MsgType);
        reject.set373_sessionRejectReason(fix::Reject::NotSupporType);
        reject.set58_text("Unknown message type");
        m_raw.push({ _input.Client, reject });
        return true;
    }

    bool Action::treatHeartbeat(SerialIn &_input)
    {
        fix::HeartBeat heartbeat;
        std::pair<bool, fix::Reject> verif = fix::HeartBeat::Verify(_input.Message);

        // need to modify user info
        if (verif.first) {
            Logger::Log("[Action] (HeartBeat) Request verification failed: "); // todo log
            verif.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            m_raw.push({ _input.Client, verif.second });
            return false;
        }
        Logger::Log("[Action] (HeartBeat) Validate from client: ", _input.Client.User);
        m_raw.push({ _input.Client, heartbeat });
        return true;
    }

}
