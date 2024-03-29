#include <future>

#include "Common/Core/Logger.hpp"
#include "Common/Core/Utils.hpp"
#include "Common/Message/Message.hpp"
#include "Server/Core/Pipeline/Action.hpp"
#include "Server/Core/meta.hpp"

namespace pip
{
    Action::Action(MarketEntry &_markets, InAction &_input, InMarketData &_data, InOutNetwork &_raw)
        : m_markets(_markets), m_input(_input), m_q_data(_data), m_q_raw(_raw)
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
        ActionInput input;

        while (m_running)
        {
            if (!m_input.empty())
            {
                input = m_input.pop_front();
                reject = fix::Header::Verify(input.Message, input.Client.User, PROVIDER_NAME, input.Client.ClientSeqNumber - 1);
                if (reject.first) {
                    reject.second.header.set56_TargetCompId(input.Client.User);
                    reject.second.header.set49_SenderCompId(PROVIDER_NAME);
                    m_q_raw.append(std::move(input.Client), std::move(reject.second));
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
                    case fix::OrderCancelRequest::cMsgType: (void)treatOrderCancelRequest(input);
                        break;
                    case fix::OrderCancelReplaceRequest::cMsgType: (void)treatOrderCancelReplaceRequest(input);
                        break;
                    case fix::Logout::cMsgType: (void)treatLogout(input);
                        break;
                    case fix::MarketDataRequest::cMsgType: (void) treatMarketDataRequest(input);
                        break;
                    default: (void)treatUnknown(input);
                        break;
                }
            }
        }
    }

    bool Action::treatLogon(ActionInput &_input)
    {
        fix::Logon logon;
        std::pair<bool, fix::Reject> verif = fix::Logon::Verify(_input.Message);

        Logger::Log("[Action] (Logon) Treating message from: ", _input.Client.User);
        if (verif.first) {
            Logger::Log("[Action] (Logon) Request verification failed: "); // todo log
            verif.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            m_q_raw.append(std::move(_input.Client), std::move(verif.second));
            return false;
        }
        _input.Client.Logged = true;
        _input.Client.User = _input.Message.at(fix::Tag::SenderCompId);
        _input.Client.SeqNumber = utils::to<size_t>(_input.Message.at(fix::Tag::MsqSeqNum));
        logon.set98_EncryptMethod("0");
        logon.set108_HeartBtInt(_input.Message.at(fix::Tag::HearBtInt));
        Logger::Log("[Action] (Logon) Request from ", _input.Client.User, " sucessfuly handle");
        m_q_raw.append(std::move(_input.Client), std::move(logon));
        return true;
    }

    bool Action::treatLogout(ActionInput &_input)
    {
        fix::Logout logout;
        std::pair<bool, fix::Reject> reject = fix::Logout::Verify(_input.Message);
        Logger::Log("[Action] (Logout) Treating message from: ", _input.Client.User);

        if (_input.Client.Logged == false) {
            Logger::Log("[Action] (Logout) Request verification failed: "); // todo log
            reject.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            reject.second.set58_text("Client not connected");
            m_q_raw.append(std::move(_input.Client), std::move(reject.second));
            return false;
        }
        _input.Client.Logged = false;
        _input.Client.User = _input.Message.at(fix::Tag::SenderCompId);
        _input.Client.Disconnect = true;
        logout.header.set56_TargetCompId(_input.Client.User);
        Logger::Log("[Action] (Logout) Request from: ", _input.Client.User, ", sucessfuly handle");
        m_q_raw.append(std::move(_input.Client), std::move(logout));
        return true;
    }

    bool Action::treatNewOrderSingle(ActionInput &_input)
    {
        MarketInput data(std::move(_input.Client));
        std::pair<bool, fix::Reject> reject = fix::NewOrderSingle::Verify(_input.Message);
        Logger::Log("[Action] (New Order Single) Treating message from: ", _input.Client.User);

        if (reject.first) {
            Logger::Log("[Action] (New Order Single) Request verification failed: "); // todo log
            reject.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            m_q_raw.append(std::move(_input.Client), std::move(reject.second));
            return false;
        }
        data.OrderData.action = OrderBook::Data::Action::Add;
        data.OrderData.type = (_input.Message.at(fix::Tag::Side) == "3") ? OrderType::Bid : OrderType::Ask;
        data.OrderData.price = utils::to<Price>(_input.Message.at(fix::Tag::Price));
        data.OrderData.order.userId = _input.Client.User;
        data.OrderData.order.orderId = utils::to<OrderId>(_input.Message.at(fix::Tag::ClOrdID));
        data.OrderData.order.quantity = utils::to<Quantity>(_input.Message.at(fix::Tag::OrderQty));
        Logger::Log("[Action] (New Order Single) Waiting for action from data: ", data.OrderData.order.userId); // todo log
        m_markets.at(_input.Message.at(fix::Tag::Symbol)).push(std::move(data));
        return true;
    }

    bool Action::treatOrderCancelRequest(ActionInput &_input)
    {
        MarketInput data(std::move(_input.Client));
        std::pair<bool, fix::Reject> reject = fix::OrderCancelRequest::Verify(_input.Message);

        if (reject.first) {
            reject.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            m_q_raw.append(std::move(_input.Client), std::move(reject.second));
            return false;
        }
        data.OrderData.action = OrderBook::Data::Action::Cancel;
        std::cout << "\n\n\n\n\n" << "[RECEIVED ORDER CANCEL REQUEST] 41 origClOrdID: '"  << _input.Message.at(fix::Tag::OrigClOrdID) << "'\n\n" << std::endl;
        std::cout << "[41]utils::to<OrderId>: '" << utils::to<OrderId>(_input.Message.at(fix::Tag::OrigClOrdID)) << "'\n\n" << std::endl;
        data.OrderData.order.orderId = utils::to<OrderId>(_input.Message.at(fix::Tag::OrigClOrdID));
        std::cout << "data.OrderData.order.orderId: '" << data.OrderData.order.orderId << "'\n\n" << std::endl;
        data.OrderData.order.userId = _input.Client.User;
        data.OrderData.type = (_input.Message.at(fix::Tag::Side) == "3") ? OrderType::Bid : OrderType::Ask;
        m_markets.at(_input.Message.at(fix::Tag::Symbol)).push(std::move(data));
        return true;
    }

    bool Action::treatOrderCancelReplaceRequest(ActionInput &_input)
    {
        MarketInput data(std::move(_input.Client));
        std::pair<bool, fix::Reject> reject = fix::OrderCancelReplaceRequest::Verify(_input.Message);
        Logger::Log("[Action] (Order Cancel Replace) Treating message from: ", _input.Client.User);

        if (reject.first) {
            Logger::Log("[Action] (Order Cancel Replace) Request verification failed: "); // todo log
            reject.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            m_q_raw.append(std::move(_input.Client), std::move(reject.second));
            return false;
        }

        std::cout << "\n\n\n\n\n" << "[RECEIVED ORDER CANCEL REQUEST] 41 origClOrdID: '"  << _input.Message.at(fix::Tag::OrigClOrdID) << "'\n\n" << std::endl;
        std::cout << "[41]utils::to<OrderId>: '" << utils::to<OrderId>(_input.Message.at(fix::Tag::OrigClOrdID)) << "'\n\n" << std::endl;
        data.OrderData.action = OrderBook::Data::Action::Modify;
        data.OrderData.order.userId = _input.Client.User;
        data.OrderData.target = _input.Message.at(fix::Tag::OrigClOrdID);
        data.OrderData.order.orderId = _input.Message.at(fix::Tag::ClOrdID);
        data.OrderData.order.quantity = utils::to<Quantity>(_input.Message.at(fix::Tag::OrderQty));
        data.OrderData.price = utils::to<Price>(_input.Message.at(fix::Tag::Price));
        data.OrderData.type = (_input.Message.at(fix::Tag::Side) == "3") ? OrderType::Bid : OrderType::Ask;
        Logger::Log("[Action] (Order Cancel Replace) Waiting for action from data: "); // todo log
        m_markets.at(_input.Message.at(fix::Tag::Symbol)).push(std::move(data));
        return true;
    }

    bool Action::treatUnknown(ActionInput &_input)
    {
        fix::Reject reject;

        Logger::Log("[Action] (New Order Single) Rejecting request from client: ", _input.Client.User, ", with request type: ", _input.Message.at(fix::Tag::MsgType));
        reject.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
        reject.set371_refTagId(fix::Tag::MsgType);
        reject.set373_sessionRejectReason(fix::Reject::NotSupporType);
        reject.set58_text("Unknown message type");
        m_q_raw.append(std::move(_input.Client), std::move(reject));
        return true;
    }

    bool Action::treatHeartbeat(ActionInput &_input)
    {
        fix::HeartBeat heartbeat;
        std::pair<bool, fix::Reject> reject = fix::HeartBeat::Verify(_input.Message);

        // need to modify user info
        if (reject.first) {
            Logger::Log("[Action] (HeartBeat) Request verification failed: "); // todo log
            reject.second.set45_refSeqNum(_input.Message.at(fix::Tag::MsqSeqNum));
            m_q_raw.append(std::move(_input.Client), std::move(reject.second));
            return false;
        }
        Logger::Log("[Action] (HeartBeat) Validate from client: ", _input.Client.User);
        m_q_raw.append(std::move(_input.Client), std::move(heartbeat));
        return true;
    }

    bool Action::treatMarketDataRequest(ActionInput &_input)
    {
        MarketDataInput sub;
        std::vector<std::string> types = utils::split<','>(_input.Message.at(fix::Tag::MDEntryType));
        std::vector<std::string> symbols = utils::split<','>(_input.Message.at(fix::Tag::Symbol));

        sub.Client = std::move(_input.Client);
        sub.Id = _input.Message.at(fix::Tag::MDReqID);
        sub.SubType = utils::to<uint8_t>(_input.Message.at(fix::Tag::SubscriptionRequestType));
        sub.Depth = utils::to<size_t>(_input.Message.at(fix::Tag::MarketDepth));
        if (sub.SubType == 1)
            sub.UpdateType = utils::to<uint8_t>(_input.Message.at(fix::Tag::MDUpdateType));
        for (const auto &_type : types) {
            if (_type == "0")
                sub.Types.push_back(OrderType::Bid);
            else
                sub.Types.push_back(OrderType::Ask);
        }
        m_q_data.push(std::move(sub));
        return true;
    }
}
