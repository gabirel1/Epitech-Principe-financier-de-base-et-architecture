#include "Message/Logon.hpp"

namespace fix
{
    Logon::Logon()
    {
        header.setMsgType(m_msgType);
    }

    Logon::~Logon() {}

    void Logon::set49_EncryptMedthod(const std::string &_val)
    {
        m_params.emplace({ "49", _val });
    }

    void Logon::set108_HeartBtInt(const std::string &_val)
    {
        m_params.emplace({ "108", _val });
    }

    void Logon::set98_EncryptMethod(const std::string &_val)
    {
        m_params.emplace({ "98", _val });
    }
}