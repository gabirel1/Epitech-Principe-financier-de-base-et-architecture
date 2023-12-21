#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    class Logout : public Message
    {
    public:
        Logout();
        ~Logout();
        /**
         * NOT REQUIRED
         */
        void set58_Text(const std::string &_val);
    protected:
        static constexpr const char *m_msgType = "5";
    };
}