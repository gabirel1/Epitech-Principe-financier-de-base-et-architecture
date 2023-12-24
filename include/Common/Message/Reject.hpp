#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    /// @brief Fix [Rject](https://www.onixs.biz/fix-dictionary/4.2/msgType_3_3.html) message class.
    class Reject : public Message
    {
        public:
            Reject();
            ~Reject() = default;

            /// @brief Reference the sequence number of the rejected fix message [45](https://www.onixs.biz/fix-dictionary/4.2/tagNum_45.html).
            /// @param _val Value assigned to it: int.
            void set45_RefSeqNum(const std::string &_val);

            /// @brief Set an optional text to explain the reason [58](https://www.onixs.biz/fix-dictionary/4.2/tagNum_58.html).
            /// @param _val Value assigned to it: string.
            void set58_Text(const std::string &_val);

            /// @brief Reference to the tag Id [371](https://www.onixs.biz/fix-dictionary/4.2/tagNum_371.html).
            /// @param _val Value assigned to it: char.
            void set371_refTagId(const std::string &_val);

            /// @brief Reference to the [fix message type](https://www.onixs.biz/fix-dictionary/4.2/tagNum_35.html) [372](https://www.onixs.biz/fix-dictionary/4.2/tagNum_371.html).
            /// @param _val Value assigned to it: char.
            void set372_refMsgType(const std::string &_val);

            /// @brief Set the reason why the fix message got rejected [373](https://www.onixs.biz/fix-dictionary/4.2/tagNum_373.html).
            /// @param _val Value assigned to it: int.
            void set373_sessionRejectReason(const std::string &_val);

        protected:
            static constexpr const char *m_msgType = "3";   ///< Message type value.
    };
}