#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    class MarketDataRequest: public Message
    {
    public:
        MarketDataRequest();
        ~MarketDataRequest();

        /**
         * @brief Unique identifier for this message.
         * Must be unique, or the ID of previous Market Data Request <V> 
         * to disable if SubscriptionRequestType <263> ='2'
         * (Disable previous Snapshot + Updates Request).
         */
        void set262_MDReqID(const std::string &_val);

        /**
         * @brief Subscription Request Type.
         * 0 = Snapshot
         * 1 = Snapshot + Updates (Subscribe)
         * 2 = Disable previous Snapshot + Updates Request (Unsubscribe)
         */
        void set263_SubscriptionRequestType(const std::string &_val);

        /**
         * @brief Market Depth.
         * 0 = Full Book
         * 1 = Top of Book
         * N > 1 = Report best N price tiers of data
         */
        void set264_MarketDepth(const std::string &_val);

        /**
         * @brief Number of MDEntryType <269> fields requested.
         */
        void set267_NoMDEntryTypes(const std::string &_val);

        /**
         * Must be the first field in this repeating group.
         * This is a list of all the types of Market Data Entries
         * that the firm requesting the Market Data is interested in receiving.
         */
        void set269_MDEntryType(const std::string &_val);

        /**
         * @brief Number of symbols (instruments) requested.
         */
        void set146_NoRelatedSym(const std::string &_val);

        /**
         * @brief Must be the first field in the repeating group.
         */
        void set55_Symbol(const std::string &_val);

    protected:
        static constexpr const char *m_msgType = "V";
    };
}