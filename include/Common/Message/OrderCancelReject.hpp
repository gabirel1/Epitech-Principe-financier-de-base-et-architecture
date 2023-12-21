#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    class OrderCancelReject: public Message
    {
    public:
        OrderCancelReject();
        ~OrderCancelReject();

        /**
         * If CxlRejReason <102>='Unknown order', specify 'NONE'.
         * @param _val
         */
        void set37_OrderID(const std::string &_val);

        /**
         * Unique order id assigned by institution to the cancel request or to the replacement order.
         * @param _val
         */
        void set11_ClOrdID(const std::string &_val);

        /**
         * ClOrdID <11> which could not be canceled/replaced. ClOrdID <11> of the previous order (NOT the initial order of the day) when canceling or replacing an order.
         * @param _val
         */
        void set41_OrigClOrdID(const std::string &_val);

        /**
         * OrdStatus <39> value after this cancel reject is applied.
         * @param _val
         */
        void set39_OrdStatus(const std::string &_val);

        /**
         * Identifies the type of request that a Cancel Reject <9> is in response to.
         * Valid values:
         * 1 = Order Cancel Request <F>
         * 2 = Order Cancel/Replace Request <G>
         */
        void set434_CxlRejReason(const std::string &_val);

    protected:
        static constexpr const char *m_msgType = "9";
    };
}