#pragma once

#include "Message/Fix.hpp"

namespace fix
{
    class Logout : public Message
    {
    public:
        Logout() { header.setMsgType(_msgType); }
        /**
         * NOT REQUIRED
         */
        void set58_Text(const std::string &_val);
    protected:
        static constexpr const char *_msgType = "5";
    };
}