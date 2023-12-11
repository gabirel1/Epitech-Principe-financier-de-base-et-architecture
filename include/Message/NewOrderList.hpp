#pragma once

#include "Message/Fix.hpp"

namespace fix
{
    class NewOrderList: public Message
    {
    public:
        NewOrderList() { header.setMsgType(_msgType); }
        /**
         * Unique identifier of list as assigned by institution, used to associate multiple individual orders.
         */
        void set66_ListID(const std::string &_val);

        /**
         * Code to identify the type of Bid Request <k>
         * @param _val
         * Valid values:
         * 1 = Non-disclosed style (e.g. US/European)
         * 2 = Disclosed style (e.g. Japanese)
         * 3 = No bidding process
         */
        void set394_BidType(const std::string &_val);

        /**
         * Total number of list entries across all messages. Should be the sum of all NoOrders <73> in each message that has repeating list entries.
         */
        void set68_TotNoOrders(const std::string &_val);

        /**
         * Number of orders to be combined for average pricing and allocation.
         */
        void set73_NoOrders(const std::string &_val);

        void set11_ClOrdID(const std::string &_val);
        void set67_ListSeqNo(const std::string &_val);
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

    protected:
        static constexpr const char *_msgType = "E";
    };
}