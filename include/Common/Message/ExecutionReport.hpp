#pragma once
#include "Common/Message/Fix.hpp"

namespace fix
{
    class ExecutionReport : public Message
    {
        public:
            ExecutionReport();
            ~ExecutionReport() = default;

            /**
             * Required to be unique for each chain of orders.
             * @param _val
             * a unique identifier for the order
             */
            void set37_OrderID(const std::string &_val);

            /**
             * Unique identifier of execution message as assigned by broker
             * (will be 0 (zero) for ExecTransType <20>='3' (Status)).
             * @param _val
             * A unique identifier for the order
             */
            void set17_ExecID(const std::string &_val);

            /**
             * Identifies transaction type
             * Valid values:
             * 0 = New
             * 1 = Cancel
             * 2 = Correct
             * 3 = Status
             * @param _val
             * Identifies transaction type
             */
            void set20_ExecTransType(const std::string &_val);

            /**
             * Describes the type of execution report. Same possible values as OrdStatus <39>.
             * @param _val
             * 0 = New
             * 1 = Partial fill
             * 2 = Fill
             * 3 = Done for day
             * 4 = Canceled
             * 5 = Replaced
             * 6 = Pending Cancel (e.g. result of Order Cancel Request <F>)
             * 7 = Stopped
             * 8 = Rejected
             * 9 = Suspended
             * A = Pending New
             * B = Calculated
             * C = Expired
             * D = Restated (ExecutionRpt sent unsolicited by sellside, with ExecRestatementReason <378> set)
             * E = Pending Replace (e.g. result of Order Cancel/Replace Request <G>)
             */
            void set150_ExecType(const std::string &_val);

            /**
             * Describes the current state of a CHAIN of orders,
             * same scope as OrderQty <38>, CumQty <14>, LeavesQty <151>, and AvgPx <6>
             * @param _val
             * Same possible values as ExecType <150>.
             */
            void set39_OrdStatus(const std::string &_val);

            /**
             * Ticker symbol
             * @param _val
             * Ticker symbol
             */
            void set55_Symbol(const std::string &_val);

            /**
             * Side of order
             * @param _val
             * valid values:
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
             * Amount of shares open for further execution.
             * If the OrdStatus <39> is Canceled, DoneForTheDay, Expired, Calculated, or Rejected
             * (in which case the order is no longer active) then LeavesQty <151> could be 0,
             * otherwise LeavesQty <151> = OrderQty <38> - CumQty <14>
             * @param _val
             * 
             */
            void set155_LeavesQty(const std::string &_val);

            /**
             * Currently executed shares for chain of orders.
             * @param _val
             * Total number of shares filled.
             */
            void set14_CumQty(const std::string &_val);

            /**
             * Calculated average price of all fills on this order.
             * @param _val
             * Calculated average price of all fills on this order.
             */
            void set6_AvgPx(const std::string &_val);

            static constexpr const char MsgType[] = "8";        ///< Message type value as string.
            static constexpr const char cMsgType = MsgType[0];  ///< Message type value as char.
    };
}