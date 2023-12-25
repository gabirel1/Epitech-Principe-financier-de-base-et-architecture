#include "Common/Message/OrderCancelRequest.hpp"

namespace fix
{
    OrderCancelRequest::OrderCancelRequest()
    {
        header.set35_MsgType(MsgType);
    }

    OrderCancelRequest::~OrderCancelRequest() {}

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