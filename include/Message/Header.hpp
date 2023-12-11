#pragma once

#include <string>

// https://www.onixs.biz/fix-dictionary/4.2/tagNum_10.html

namespace fix
{
    class Header
    {
        public:
            std::string BeginString;    // 8 --> Init at the beginning of the session
            std::string BodyLength;     // 9
            std::string MsgType;        // 35
            std::string SenderCompId;   // 49 --> Init at the beginning of the session
            std::string TargetCompId;   // 56 --> Init at the beginning of the session
            std::string MsgSeqNum = "0";      // 34
            std::string SendingTime;    // 56

            void setBodyLength(const std::string &_body);
            void setMsgType(const std::string &_val);
            void setSendingTime();
            std::string getPartialHeader() const;

            operator std::string() const;
    };
}