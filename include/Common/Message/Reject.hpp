#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    class Reject : public Message
    {
        public:
            void set372_refMsgType(const std::string &_val);
            void set373_sessionRejectReason(const std::string &_val);
            void set58_Text(const std::string &_val);
    };
}