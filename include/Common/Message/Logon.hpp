#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    class Logon : public Message
    {
        void set49_EncryptMedthod(const std::string &_val);
        void set108_HeartBtInt(const std::string &_val);
    };
}