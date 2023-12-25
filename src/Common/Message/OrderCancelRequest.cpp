#include "Common/Message/OrderCancelRequest.hpp"
#include "Common/Message/Utils.hpp"

namespace fix
{
    OrderCancelRequest::OrderCancelRequest()
    {
        header.set35_MsgType(m_msgType);
    }

    OrderCancelRequest::~OrderCancelRequest() {}

    std::pair<bool, Reject> OrderCancelRequest::Verify(Serializer::AnonMessage &_msg)
    {
        std::pair<bool, Reject> reject{ true, {} };
        std::vector<std::string> requiredFields = { "41", "11", "55", "54", "60" };

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
        if (_msg.at("54") != "3" && _msg.at("54") != "4") {
            reject.first = true;
            reject.second.set371_refTagId("54");
            reject.second.set373_sessionRejectReason("5");
            reject.second.set58_Text("Value not supported (only 3 or 4 are allowed)");
        }
        return reject;
    }

    void OrderCancelRequest::set41_OrigClOrdID(const std::string &_val)
    {
        m_params.emplace({ "41", _val });
    }

    void OrderCancelRequest::set11_ClOrdID(const std::string &_val)
    {
        m_params.emplace({ "11", _val });
    }

    void OrderCancelRequest::set55_Symbol(const std::string &_val)
    {
        m_params.emplace({ "55", _val });
    }

    void OrderCancelRequest::set54_Side(const std::string &_val)
    {
        m_params.emplace({ "54", _val });
    }

    void OrderCancelRequest::set60_TransactTime(const std::string &_val)
    {
        m_params.emplace({ "60", _val });
    }
}