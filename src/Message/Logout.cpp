#include "Message/Logout.hpp"

namespace fix
{
    void Logout::set58_Text(const std::string &_val)
    {
        m_params.emplace({ "58", _val });
    }
}