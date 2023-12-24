#pragma once

#include <string>
#include "Common/Message/Serializer.hpp"

namespace fix
{
    class Reject;

    /// @brief Fix [Header](https://www.onixs.biz/fix-dictionary/4.2/compBlock_StandardHeader.html) class.
    class Header
    {
        public:
            Header() = default;
            ~Header() = default;

            static std::pair<bool, Reject> Verify(Serializer::AnonMessage &_msg);

            void set49_SenderCompId(const std::string &_val);
            void set56_TargetCompId(const std::string &_val);

            void setBodyLength(const std::string &_len);
            void setBodyLength(const size_t &_len);

            /// @brief Set the type of message send [35](https://www.onixs.biz/fix-dictionary/4.2/tagNum_35.html).
            /// @param _val Value assigned to it: char.
            void set35_MsgType(const std::string &_val);

            void setSeqNum(const std::string &_val);
            void setSeqNum(const size_t &_val);

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
            std::string MsgType{};                  // 35
            std::string SenderCompId{};             // 49 --> Init at the beginning of the session
            std::string TargetCompId{};             // 56 --> Init at the beginning of the session
            std::string MsgSeqNum = "0";            // 34
            std::string SendingTime{};              // 56
    };
}