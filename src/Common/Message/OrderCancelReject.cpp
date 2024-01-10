#include "Common/Message/OrderCancelReject.hpp"

namespace fix
{
    OrderCancelReject::OrderCancelReject()
    {
        header.set35_MsgType(MsgType);
    }

    void OrderCancelReject::set37_OrderID(const std::string &_val)
    {
        m_params.emplace({ "37", _val });
    }

    void OrderCancelReject::set11_ClOrdID(const std::string &_val)
    {
        m_params.emplace({ "11", _val });
    }

    void OrderCancelReject::set41_OrigClOrdID(const std::string &_val)
    {
        m_params.emplace({ "41", _val });
    }

    void OrderCancelReject::set39_OrdStatus(const std::string &_val)
    {
        m_params.emplace({ "39", _val });
    }

    void OrderCancelReject::set434_CxlRejReason(const std::string &_val)
    {
        m_params.emplace({ "434", _val });
    }
}