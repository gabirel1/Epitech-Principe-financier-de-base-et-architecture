#pragma once

#include "Message/Fix.hpp"

namespace fix
{
    class Logon : public Message
    {
    public:
        Logon() { header.setMsgType(_msgType); }
        // doesn't exists :woman_shrugging:
        void set49_EncryptMedthod(const std::string &_val);
        void set98_EncryptMethod(const std::string &_val);
        void set108_HeartBtInt(const std::string &_val);
    protected:
        static constexpr const char *_msgType = "A";
    };
}