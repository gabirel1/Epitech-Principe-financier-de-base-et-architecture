#include "Common/Core/Utils.hpp"
#include "Common/Message/OrderCancelRequest.hpp"
#include "Common/Message/Reject.hpp"
#include "Common/Message/Tag.hpp"

namespace fix
{
    OrderCancelRequest::OrderCancelRequest()
    {
        header.set35_MsgType(MsgType);
    }

    OrderCancelRequest::~OrderCancelRequest() {}

    std::pair<bool, Reject> OrderCancelRequest::Verify(Serializer::AnonMessage &_msg)
    {
        // need to verify transaction time, symbol
        std::pair<bool, Reject> reject = utils::Has<Tag::ClOrdID, Tag::OrigClOrdID, Tag::Side, Tag::Symbol, Tag::TransactTime>(_msg);

        if (reject.first) {
            return reject;
        } else if (!utils::is_numeric(_msg.at(Tag::ClOrdID))) {
            reject.second.set371_refTagId(Tag::ClOrdID);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Not supported order Id");
        } else if (!utils::is_numeric(_msg.at(Tag::OrigClOrdID))) {
            reject.second.set371_refTagId(Tag::OrigClOrdID);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Not supported origin order Id");
        } else if (_msg.at(Tag::Side) != "3" && _msg.at(Tag::Side) != "4") {
            reject.first = true;
            reject.second.set371_refTagId(Tag::Side);
            reject.second.set373_sessionRejectReason(Reject::ValueOORange);
            reject.second.set58_text("Value not supported (only 3 or 4 are allowed)");
        } else {
            reject.first = false;
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