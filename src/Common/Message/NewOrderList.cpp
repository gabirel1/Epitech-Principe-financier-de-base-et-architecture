#include "Common/Message/NewOrderList.hpp"

namespace fix
{
    NewOrderList::NewOrderList()
    {
        header.setMsgType(m_msgType);
    }

    NewOrderList::~NewOrderList() {}

    void NewOrderList::set66_ListID(const std::string &_val)
    {
        m_params.emplace({ "66", _val });
    }

    void NewOrderList::set394_BidType(const std::string &_val)
    {
        m_params.emplace({ "394", _val });
    }

    void NewOrderList::set68_TotNoOrders(const std::string &_val)
    {
        m_params.emplace({ "68", _val });
    }

    void NewOrderList::set73_NoOrders(const std::string &_val)
    {
        m_params.emplace({ "73", _val });
    }

    void NewOrderList::set11_ClOrdID(const std::string &_val)
    {
        m_params.emplace({ "11", _val });
    }

    void NewOrderList::set67_ListSeqNo(const std::string &_val)
    {
        m_params.emplace({ "67", _val });
    }

    void NewOrderList::set55_Symbol(const std::string &_val)
    {
        m_params.emplace({ "55", _val });
    }

    void NewOrderList::set54_Side(const std::string &_val)
    {
        m_params.emplace({ "54", _val });
    }
}