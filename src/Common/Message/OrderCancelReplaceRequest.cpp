#include "Common/Message/OrderCancelReplaceRequest.hpp"
#include "Common/Message/Utils.hpp"

namespace fix
{
    OrderCancelReplaceRequest::OrderCancelReplaceRequest()
    {
        header.set35_MsgType(m_msgType);
    }

    std::pair<bool, Reject> OrderCancelReplaceRequest::Verify(Serializer::AnonMessage &_msg)
    {
        std::pair<bool, Reject> reject{ true, {} };
        std::vector<std::string> requiredFields = { "41", "11", "21", "55", "54", "60", "40", "38", "44" };

        reject.second.set372_refMsgType(m_msgType);
        reject.first = false;

        for (auto &field : requiredFields) {
            if (!_msg.contains(field)) {
                reject.first = true;
                reject.second.set371_refTagId(field);
                reject.second.set373_sessionRejectReason("1");
                reject.second.set58_Text("Unable to find required field");
                return reject;
            }
        }
        if (_msg.at("21") != "3") {
            reject.first = true;
            reject.second.set371_refTagId("21");
            reject.second.set373_sessionRejectReason("5");
            reject.second.set58_Text("Value not supported");
        } else if (_msg.at("54") != "3" && _msg.at("54") != "4") {
            reject.first = true;
            reject.second.set371_refTagId("54");
            reject.second.set373_sessionRejectReason("5");
            reject.second.set58_Text("Value not supported (only 3 or 4 are allowed)");
        } else if (_msg.at("40") != "2") {
            reject.first = true;
            reject.second.set371_refTagId("40");
            reject.second.set373_sessionRejectReason("5");
            reject.second.set58_Text("Value not supported (only 2 is allowed)");
        } else if (!utils::is_double(_msg.at("38"))) {
            reject.first = true;
            reject.second.set371_refTagId("38");
            reject.second.set373_sessionRejectReason("6");
            reject.second.set58_Text("Value not supported (only double is allowed)");
        } else if (!utils::is_double(_msg.at("44"))) {
            reject.first = true;
            reject.second.set371_refTagId("44");
            reject.second.set373_sessionRejectReason("6");
            reject.second.set58_Text("Value not supported (only double is allowed)");
        }
        return reject;
    }

    OrderCancelReplaceRequest::~OrderCancelReplaceRequest() {}

    void OrderCancelReplaceRequest::set41_OrigClOrdID(const std::string &_val)
    {
        m_params.emplace({ "41", _val });
    }

    void OrderCancelReplaceRequest::set11_ClOrdID(const std::string &_val)
    {
        m_params.emplace({ "11", _val });
    }

    void OrderCancelReplaceRequest::set21_HandlInst(const std::string &_val)
    {
        m_params.emplace({ "21", _val });
    }

    void OrderCancelReplaceRequest::set55_Symbol(const std::string &_val)
    {
        m_params.emplace({ "55", _val });
    }

    void OrderCancelReplaceRequest::set54_Side(const std::string &_val)
    {
        m_params.emplace({ "54", _val });
    }

    void OrderCancelReplaceRequest::set60_TransactTime(const std::string &_val)
    {
        m_params.emplace({ "60", _val });
    }

    void OrderCancelReplaceRequest::set40_OrdType(const std::string &_val)
    {
        m_params.emplace({ "40", _val });
    }

    void OrderCancelReplaceRequest::set38_OrderQty(const std::string &_val)
    {
        m_params.emplace({ "38", _val });
    }

    void OrderCancelReplaceRequest::set44_Price(const std::string &_val)
    {
        m_params.emplace({ "44", _val });
    }
}