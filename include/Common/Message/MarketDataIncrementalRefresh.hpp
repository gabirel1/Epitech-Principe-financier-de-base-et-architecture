#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    class MarketDataIncrementalRefresh : public Message
    {
        public:
            MarketDataIncrementalRefresh();
            ~MarketDataIncrementalRefresh() = default;

            /**
             * @brief Number of entries following.
             */
            void set268_NoMDEntries(const std::string &_val);

            /**
             * @brief MDEntryType Must be the first field in this repeating group.
             */
            void set279_MDUpdateAction(const std::string &_val);

            static constexpr const char MsgType[] = "X";        ///< Message type value as string.
            static constexpr const char cMsgType = MsgType[0];  ///< Message type value as char.
    };
}
