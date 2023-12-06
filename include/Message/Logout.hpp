#pragma once

#include "Message/Fix.hpp"

namespace fix
{
    class Logout : public Message
    {
        /**
         * NOT REQUIRED
         */
        void set58_Text(const std::string &_val);
    };
}