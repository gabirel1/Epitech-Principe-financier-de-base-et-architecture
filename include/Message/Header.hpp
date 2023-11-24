#pragma once

#include <string>

namespace fix
{
    class Header
    {
        public:
            std::string BeginString;
            std::string BodyLength;
            std::string MsgType;
            std::string SenderCompId;
            std::string TargetCompId;
            std::string MsgSeqNum;
            std::string SendingTime;

            operator std::string() const;
    };
}