#include "Common/Message/OrderCancelReject.hpp"
#include "Common/Message/Tag.hpp"

namespace fix
{
    OrderCancelReject::OrderCancelReject()
    {
        header.set35_MsgType(MsgType);
    }

    void OrderCancelReject::set11_clOrdID(const std::string &_val)
    {
        m_params.emplace({ fix::Tag::ClOrdID, _val });
    }

    void OrderCancelReject::set37_orderID(const std::string &_val)
    {
        m_params.emplace({ fix::Tag::OrderID, _val });
    }

    void OrderCancelReject::set39_ordStatus(const std::string &_val)
    {
        m_params.emplace({ fix::Tag::OrdStatus, _val });
    }

    void OrderCancelReject::set41_origClOrdID(const std::string &_val)
    {
        m_params.emplace({ fix::Tag::OrigClOrdID, _val });
    }

    void OrderCancelReject::set58_text(const std::string &_val)
    {
        m_params.emplace({ Tag::Text, _val });
    }

    void OrderCancelReject::set434_cxlRejReason(const std::string &_val)
    {
        m_params.emplace({ "434", _val });
    }
}