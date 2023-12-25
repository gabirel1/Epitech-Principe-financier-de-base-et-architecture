#pragma once
#include "Common/Message/Fix.hpp"
#include "Common/Message/Reject.hpp"

namespace fix
{
    /**
     * @brief Order Cancel Request
     * The Order Cancel Request <F> message requests the cancelation of all of the remaining quantity of an existing order.
     * Note that the Order Cancel/Replace Request <G> should be used to partially cancel (reduce) an order.
     */
    class OrderCancelRequest : public Message
    {
        public:
            OrderCancelRequest();
            ~OrderCancelRequest();

            /// @brief Verify if the logon message receive is correctly formated.
            /// @param _msg Message to check.
            /// @return If the first element is true then second is set, otherwise it rigly formated.
            static std::pair<bool, Reject> Verify(Serializer::AnonMessage &_msg);

            /**
             * Unique identifier of the order to cancel.
             * @param _val
             * Unique identifier of the order
             * @brief ClOrdID <11> of the order to be canceled.
             */
            void set41_OrigClOrdID(const std::string &_val);

            /**
             * Unique ID of cancel request as assigned by the institution.
             * @param _val
             * Unique identifier for Order as assigned by institution (identified by SenderCompID <49> or OnBehalfOfCompID <115> as appropriate).
             */
            void set11_ClOrdID(const std::string &_val);

            /**
             * Ticker symbol
             * @param _val
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
             * @param _val
             * Time this order request was initiated/released by the trader or trading system.
             */
            void set60_TransactTime(const std::string &_val);

            static constexpr const char MsgType[] = "F";         ///< Message type value as string.
            static constexpr const char cMsgType = MsgType[0];  ///< Message type value as char.
    };
}