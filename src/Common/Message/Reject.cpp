#include "Common/Message/Reject.hpp"

namespace fix
{
    Reject::Reject()
    {
        header.set35_MsgType(m_msgType);
    }

    void Reject::set45_RefSeqNum(const std::string &_val)
    {
        m_params.emplace({ "45", _val });
    }

    void Reject::set58_Text(const std::string &_val)
    {
        m_params.emplace({ "58", _val });
    }

    void Reject::set371_refTagId(const std::string &_val)
    {
        m_params.emplace({ "371", _val });
    }

    void Reject::set372_refMsgType(const std::string &_val)
    {
        m_params.emplace({ "372", _val });
    }

    void Reject::set373_sessionRejectReason(const std::string &_val)
    {
        m_params.emplace({ "373", _val });
    }
}