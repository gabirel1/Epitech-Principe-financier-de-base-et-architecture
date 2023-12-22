#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    class Logon : public Message
    {
        public:
            Logon();
            ~Logon() = default;
            
            void set98_EncryptMethod(const std::string &_val);
            void set108_HeartBtInt(const std::string &_val);

        protected:
            static constexpr const char *m_msgType = "A";
    };
}