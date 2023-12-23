#pragma once

#include <string>

// https://www.onixs.biz/fix-dictionary/4.2/tagNum_10.html

namespace fix
{
    class Header
    {
        public:
            Header() = default;
            ~Header() = default;

            void set49_SenderCompId(const std::string &_val);
            void set56_TargetCompId(const std::string &_val);

            void setBodyLength(const std::string &_len);
            void setMsgType(const std::string &_val);
            void setSeqNum(const std::string &_val);

            operator std::string() const;

        protected:
            friend class Message;

            void setSendingTime();
            [[deprecated("Use setSeqNum() before calling operator std::string instead")]]
            void updateMsgSeqNum();

            std::string getPartialHeader() const;

        private:
            std::string BeginString = "FIX.4.2";    // 8 --> Init at the beginning of the session
            std::string BodyLength = "0";           // 9
            std::string MsgType{};                    // 35
            std::string SenderCompId{};   // 49 --> Init at the beginning of the session
            std::string TargetCompId{};   // 56 --> Init at the beginning of the session
            std::string MsgSeqNum = "0";            // 34
            std::string SendingTime{};                // 56
    };
}