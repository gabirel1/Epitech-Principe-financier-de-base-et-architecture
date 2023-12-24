#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    /// @brief Fix [Logout](https://www.onixs.biz/fix-dictionary/4.2/msgType_5_5.html) message class.
    class Logout : public Message
    {
        public:
            Logout();
            ~Logout() = default;

            /// @brief Verify if the Logout message receive is correctly formated.
            /// @param _msg Message to check.
            /// @return The first element will always be false.
            static std::pair<bool, Reject> Verify(Serializer::AnonMessage &_msg);

        protected:
            static constexpr const char *m_msgType = "5";
    };
}