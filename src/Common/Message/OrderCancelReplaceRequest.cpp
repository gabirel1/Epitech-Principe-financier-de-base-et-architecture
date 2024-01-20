#include "Common/Core/Utils.hpp"
#include "Common/Message/OrderCancelReplaceRequest.hpp"
#include "Common/Message/Tag.hpp"
#include <iostream>

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

        if (!reject.first)
            reject = verify_all<Tag::ClOrdID, Tag::HandlInst, Tag::OrderQty, Tag::OrdType,
                Tag::OrigClOrdID, Tag::Price, Tag::Side, Tag::Symbol, Tag::TransactTime>(_msg);
        reject.second.set45_refSeqNum(OrderCancelReplaceRequest::MsgType);
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