#pragma once

#include "Message/Fix.hpp"

namespace fix
{
    /**
     * @brief Order Cancel Replace Request (G)
     * Do not use this message to cancel the remaining quantity of an outstanding order, use the Cancel Request <F> message for this purpose.
     */
    class OrderCancelReplaceRequest : public Message
    {
    public:
        OrderCancelReplaceRequest();
        ~OrderCancelReplaceRequest();

        /**
         * ClOrdID <11> of the previous order (NOT the initial order of the day) when canceling or replacing an order.
         * Used to identify the previous order in cancel and cancel/replace requests.
         * @param _val
         * Unique identifier of the order
         */
        void set41_OrigClOrdID(const std::string &_val);

        /**
         * Unique identifier of replacement order as assigned by institution.
         * Note that this identifier will be used in ClOrdID <11> field of the Cancel Reject <9> message if the replacement request is rejected.
         * @param _val
         * Unique identifier of replacement order
         */
        void set11_ClOrdID(const std::string &_val);

        /**
         * Instructions for order handling on Broker trading floor
         * Valid values:
         * 1 = Automated execution order, private, no Broker intervention
         * 2 = Automated execution order, public, Broker intervention OK
         * 3 = Manual order, best execution
         */
        void set21_HandlInst(const std::string &_val);

        /**
         * Ticker symbol - Must match original order
         * @param _val
         * Ticker symbol
         */
        void set55_Symbol(const std::string &_val);

        /**
         * Must match original side, however, Buy and Buy Minus can be interchanged as well as Sell and Sell Plus
         * @param _val
         * Valid values:
         * 1 = Buy
         * 2 = Sell
         * 3 = Buy minus
         * 4 = Sell plus
         * 5 = Sell short
         * 6 = Sell short exempt
         * 7 = Undisclosed (valid for IOI and List Order messages only)
         * 8 = Cross (orders where counterparty is an exchange, valid for all messages except IOIs)
         * 9 = Cross short
         */
        void set54_Side(const std::string &_val);

        /**
         * Time this order request was initiated/released by the trader or trading system.
         * @param _val
         * Time this order request was initiated/released by the trader or trading system.
         */
        void set60_TransactTime(const std::string &_val);

        /**
         * Order type.
         * @param _val
         * Valid values:
         * 1 = Market
         * 2 = Limit
         * 3 = Stop
         * 4 = Stop limit
         * 5 = Market on close (No longer used)
         * 6 = With or without
         * 7 = Limit or better
         * 8 = Limit with or without
         * 9 = On basis
         * A = On close (No longer used)
         * B = Limit on close (No longer used)
         * C = Forex - Market (No longer used)
         * D = Previously quoted
         * E = Previously indicated
         * F = Forex - Limit (No longer used)
         * G = Forex - Swap
         * H = Forex - Previously Quoted (No longer used)
         * I = Funari (Limit Day Order with unexecuted portion handled as Market On Close. E.g. Japan)
         * P = Pegged
         */
        void set40_OrdType(const std::string &_val);

    protected:
        static constexpr const char *m_msgType = "G";
    };
}