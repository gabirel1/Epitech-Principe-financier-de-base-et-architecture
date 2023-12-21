#include "Common/Message/Logout.hpp"

namespace fix
{
    Logout::Logout()
    {
        header.setMsgType(m_msgType);
    }

    Logout::~Logout() {}

    void Logout::set58_Text(const std::string &_val)
    {
        m_params.emplace({ "58", _val });
    }
}