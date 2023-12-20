#pragma once

#include "Message/Fix.hpp"

namespace fix
{
    class MarketDataIncrementalRefresh : public Message
    {
    public:
        MarketDataIncrementalRefresh();
        ~MarketDataIncrementalRefresh();

        /**
         * @brief Number of entries following.
         */
        void set268_NoMDEntries(const std::string &_val);

        /**
         * @brief MDEntryType Must be the first field in this repeating group.
         */
        void set279_MDUpdateAction(const std::string &_val);
    protected:
        static constexpr const char *m_msgType = "X";
    };
}
