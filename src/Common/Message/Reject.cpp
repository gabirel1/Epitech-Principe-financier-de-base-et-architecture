#include "Common/Message/Reject.hpp"

namespace fix
{
    void Reject::set372_refMsgType(const std::string &_val)
    {
        m_params.emplace({ "372", _val });
    }

    void Reject::set373_sessionRejectReason(const std::string &_val)
    {
        m_params.emplace({ "373", _val });
    }

    void Reject::set58_Text(const std::string &_val)
    {
        m_params.emplace({ "58", _val });
    }
}