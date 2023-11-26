#pragma once

#include <string>

// https://www.onixs.biz/fix-dictionary/4.2/tagNum_10.html

namespace fix
{
    class Header
    {
        public:
            std::string BeginString;    // 8
            std::string BodyLength;     // 9
            std::string MsgType;        // 35
            std::string SenderCompId;   // 49
            std::string TargetCompId;   // 56
            std::string MsgSeqNum;      // 34
            std::string SendingTime;    // 56

            inline operator std::string() const;
    };
}