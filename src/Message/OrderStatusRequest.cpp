#include "Message/OrderStatusRequest.hpp"

namespace fix
{
    OrderStatusRequest::OrderStatusRequest()
    {
        header.setMsgType(m_msgType);
    }

    OrderStatusRequest::~OrderStatusRequest() {}

    // void OrderStatusRequest::set37_OrderID(const std::string &_val)
    // {
    //     m_params.emplace({ "37", _val });
    // }

    void OrderStatusRequest::set11_ClOrdID(const std::string &_val)
    {
        m_params.emplace({ "11", _val });
    }

    // void OrderStatusRequest::set41_OrigClOrdID(const std::string &_val)
    // {
    //     m_params.emplace({ "41", _val });
    // }

    void OrderStatusRequest::set55_Symbol(const std::string &_val)
    {
        m_params.emplace({ "55", _val });
    }

    void OrderStatusRequest::set54_Side(const std::string &_val)
    {
        m_params.emplace({ "54", _val });
    }
}