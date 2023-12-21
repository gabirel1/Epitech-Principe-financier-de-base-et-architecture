#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    /**
     * @brief Market Data messages are used as the response to a Market Data Request <V> message.
    */
    class MarketDataSnapshotFullRefresh: public Message
    {
    public:
        MarketDataSnapshotFullRefresh();
        ~MarketDataSnapshotFullRefresh();

        /**
         * @brief Set the 55 Symbol object
         */
        void set55_Symbol(const std::string &_val);

        /**
         * @brief Number of entries following.
         */
        void set268_NoMDEntries(const std::string &_val);

        /**
         * @brief MDEntryType Must be the first field in this repeating group.
         */
        void set269_MDEntryType(const std::string &_val);

        /**
         * @brief MDEntryPx Price of the Market Data Entry.
         */
        void set270_MDEntryPx(const std::string &_val);

    protected:
        static constexpr const char *m_msgType = "W";
    };
}