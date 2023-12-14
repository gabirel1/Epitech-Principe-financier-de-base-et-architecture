#pragma once

#include "Message/Fix.hpp"

namespace fix
{
    class NewOrderSingle : public Message
    {
    public:
        NewOrderSingle();
        ~NewOrderSingle();
        /**
         * Unique identifier of the order as assigned by institution.
         */
        void set11_ClOrdID(const std::string &_val);

        /**
         * 21 - HandlInst
         * Instructions for order handling on Broker trading floor
         * Valid values:
         * 1 = Automated execution order, private, no Broker intervention
         * 2 = Automated execution order, public, Broker intervention OK
         * 3 = Manual order, best execution
         */
        void set21_HandlInst(const std::string &_val);

        /**
         * Ticker symbol
         */
        void set55_Symbol(const std::string &_val);

        /**
         * Side of order
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
         * 5 = Market on close
         * 6 = With or without
         * 7 = Limit or better
         * 8 = Limit with or without
         * 9 = On basis
         * A = On close
         * B = Limit on close
         * C = Forex - Market
         * D = Previously quoted
         * E = Previously indicated
         * F = Forex - Limit
         * G = Forex - Swap
         * H = Forex - Previously Quoted
         * I = Funari (Limit Day Order with unexecuted portion handled as Market On Close. e.g. Japan)
         * P = Pegged
         */
        void set40_OrdType(const std::string &_val);

    protected:
        static constexpr const char *m_msgType = "D";
    };
}