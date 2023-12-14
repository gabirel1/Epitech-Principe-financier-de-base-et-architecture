#pragma once

#include <string>

// https://www.onixs.biz/fix-dictionary/4.2/tagNum_10.html

namespace fix
{
    class Header
    {
        public:
            Header();
            Header(fix::Header &_header);
            ~Header();
            void setBodyLength(const std::size_t &_len);
            void setMsgType(const std::string &_val);
            void updateMsgSeqNum();
            void setSendingTime();
            std::string getPartialHeader() const;
            operator std::string() const;

        protected:
            std::string BeginString = "FIX.4.2";    // 8 --> Init at the beginning of the session
            std::string BodyLength = "0";           // 9
            std::string MsgType;                    // 35
            std::string SenderCompId = "CLIENT";   // 49 --> Init at the beginning of the session
            std::string TargetCompId = "TARGET";   // 56 --> Init at the beginning of the session
            std::string MsgSeqNum = "0";            // 34
            std::string SendingTime;                // 56
    };
}