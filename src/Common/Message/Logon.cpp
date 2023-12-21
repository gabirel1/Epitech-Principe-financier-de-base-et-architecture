#include "Common/Message/Logon.hpp"

namespace fix
{
    void Logon::set49_EncryptMedthod(const std::string &_val)
    {
        m_params.emplace({ "49", _val });
    }

    void Logon::set108_HeartBtInt(const std::string &_val)
    {
        m_params.emplace({ "108", _val });
    }
}