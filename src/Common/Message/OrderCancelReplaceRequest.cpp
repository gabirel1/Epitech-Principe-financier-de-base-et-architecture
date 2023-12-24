#include "Common/Message/OrderCancelReplaceRequest.hpp"

namespace fix
{
    OrderCancelReplaceRequest::OrderCancelReplaceRequest()
    {
        header.set35_MsgType(m_msgType);
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