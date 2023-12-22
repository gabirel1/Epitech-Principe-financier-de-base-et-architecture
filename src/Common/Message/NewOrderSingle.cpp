#include "Common/Message/NewOrderSingle.hpp"

namespace fix
{
    NewOrderSingle::NewOrderSingle()
    {
        header.setMsgType(m_msgType);
    }

    void NewOrderSingle::set11_ClOrdID(const std::string &_val)
    {
        m_params.emplace({ "11", _val });
    }

    void NewOrderSingle::set21_HandlInst(const std::string &_val)
    {
        m_params.emplace({ "21", _val });
    }

    void NewOrderSingle::set55_Symbol(const std::string &_val)
    {
        m_params.emplace({ "55", _val });
    }

    void NewOrderSingle::set54_Side(const std::string &_val)
    {
        m_params.emplace({ "54", _val });
    }

    void NewOrderSingle::set60_TransactTime(const std::string &_val)
    {
        m_params.emplace({ "60", _val });
    }

    void NewOrderSingle::set40_OrdType(const std::string &_val)
    {
        m_params.emplace({ "40", _val });
    }
}