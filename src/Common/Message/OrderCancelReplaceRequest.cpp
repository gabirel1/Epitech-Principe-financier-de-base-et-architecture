#include "Common/Core/Utils.hpp"
#include "Common/Message/OrderCancelReplaceRequest.hpp"
#include "Common/Message/Tag.hpp"

namespace fix
{
    OrderCancelReplaceRequest::OrderCancelReplaceRequest()
    {
        header.set35_MsgType(MsgType);
    }

    std::pair<bool, Reject> OrderCancelReplaceRequest::Verify(Serializer::AnonMessage &_msg)
    {
        // need to verify transaction time, symbol
        std::pair<bool, Reject> reject = utils::Has<Tag::ClOrdID, Tag::HandlInst,
            Tag::OrderQty, Tag::OrdType, Tag::OrigClOrdID, Tag::Price, Tag::Side, Tag::Symbol, Tag::TransactTime>(_msg);

        reject.second.set45_refSeqNum(OrderCancelReplaceRequest::MsgType);
        if (reject.first) {
            return reject;
        } else if (!utils::is_numeric(_msg.at(Tag::ClOrdID))) {
            reject.second.set371_refTagId(Tag::ClOrdID);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Not supported order Id");
        } else if (_msg.at(Tag::HandlInst) != "3") {
            reject.second.set371_refTagId(Tag::HandlInst);
            reject.second.set373_sessionRejectReason(Reject::ValueOORange);
            reject.second.set58_text("Not supported order Id");
        } else if (!utils::is_double(_msg.at(Tag::OrderQty))) {
            reject.second.set371_refTagId(Tag::OrderQty);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Quantity should be a double");
        } else if (_msg.at(Tag::OrdType) != "2") {
            reject.second.set371_refTagId(Tag::OrdType);
            reject.second.set373_sessionRejectReason(Reject::ValueOORange);
            reject.second.set58_text("Order type not supported");
        } else if (!utils::is_numeric(_msg.at(Tag::OrigClOrdID))) {
            reject.second.set371_refTagId(Tag::OrigClOrdID);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Price should be a double");
        } else if (!utils::is_double(_msg.at(Tag::Price))) {
            reject.second.set371_refTagId(Tag::Price);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Price should be a double");
        } else if (_msg.at(Tag::Side) != "3" && _msg.at(Tag::Side) != "4") {
            reject.second.set371_refTagId(Tag::Side);
            reject.second.set373_sessionRejectReason(Reject::ValueOORange);
            reject.second.set58_text("Trading side not supported");
        } else {
            reject.first = false;
        }
        return reject;
    }

    void OrderCancelReplaceRequest::set11_clOrdID(const std::string &_val)
    {
        m_params.emplace({ Tag::ClOrdID, _val });
    }

    void OrderCancelReplaceRequest::set21_handlInst(const std::string &_val)
    {
        m_params.emplace({ Tag::HandlInst, _val });
    }

    void OrderCancelReplaceRequest::set38_orderQty(const std::string &_val)
    {
        m_params.emplace({ Tag::OrderQty, _val });
    }

    void OrderCancelReplaceRequest::set40_ordType(const std::string &_val)
    {
        m_params.emplace({ Tag::OrdType, _val });
    }

    void OrderCancelReplaceRequest::set41_origClOrdID(const std::string &_val)
    {
        m_params.emplace({ Tag::OrigClOrdID, _val });
    }

    void OrderCancelReplaceRequest::set44_price(const std::string &_val)
    {
        m_params.emplace({ Tag::Price, _val });
    }

    void OrderCancelReplaceRequest::set54_side(const std::string &_val)
    {
        m_params.emplace({ Tag::Side, _val });
    }

    void OrderCancelReplaceRequest::set55_symbol(const std::string &_val)
    {
        m_params.emplace({ Tag::Symbol, _val });
    }

    void OrderCancelReplaceRequest::set60_transactTime(const std::string &_val)
    {
        m_params.emplace({ Tag::TransactTime, _val });
    }
}