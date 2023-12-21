#include "Common/Message/ExecutionReport.hpp"

namespace fix
{
    ExecutionReport::ExecutionReport()
    {
        header.setMsgType(m_msgType);
    }

    ExecutionReport::~ExecutionReport() {}

    void ExecutionReport::set37_OrderID(const std::string &_val)
    {
        m_params.emplace({ "37", _val });
    }

    void ExecutionReport::set17_ExecID(const std::string &_val)
    {
        m_params.emplace({ "17", _val });
    }

    void ExecutionReport::set20_ExecTransType(const std::string &_val)
    {
        m_params.emplace({ "20", _val });
    }

    void ExecutionReport::set150_ExecType(const std::string &_val)
    {
        m_params.emplace({ "150", _val });
    }

    void ExecutionReport::set39_OrdStatus(const std::string &_val)
    {
        m_params.emplace({ "39", _val });
    }

    void ExecutionReport::set55_Symbol(const std::string &_val)
    {
        m_params.emplace({ "55", _val });
    }

    void ExecutionReport::set54_Side(const std::string &_val)
    {
        m_params.emplace({ "54", _val });
    }

    void ExecutionReport::set155_LeavesQty(const std::string &_val)
    {
        m_params.emplace({ "155", _val });
    }

    void ExecutionReport::set14_CumQty(const std::string &_val)
    {
        m_params.emplace({ "14", _val });
    }

    void ExecutionReport::set6_AvgPx(const std::string &_val)
    {
        m_params.emplace({ "6", _val });
    }
}