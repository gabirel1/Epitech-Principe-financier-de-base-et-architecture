#pragma once

#include "Client/Processor/IProcessor.hpp"

namespace proc
{
    class User : public proc::IMessage, public proc::IEntry
    {
        public:
            virtual std::optional<fix::Message> process(fix::Serializer::AnonMessage &_msg, Context &_ctx) override final;
            virtual std::optional<fix::Message> process(const std::string &_entry, Context &_ctx) override final;

            virtual std::optional<fix::Message> build(char _tag, Context &_ctx) const override final;

        protected:
            fix::Message buildLogon(const UserId &_id) const;
            fix::Message buildLogout() const;
            fix::Message buildNewOrder(const std::string &_orderId, const std::string &_quantity, const std::string &_price, const std::string &_side, const std::string &_symbol) const;
            fix::Message buildOrderCancel(const std::string &_cancelOrderId, const std::string &_targetOrderId, const std::string &_symbol, const std::string &_side) const;
            fix::Message buildOrderCancelReplace(const std::string &_cancelReplaceOrderId, const std::string &_targetOrderId, const std::string &_quantity, const std::string &_price, const std::string &_side, const std::string &_symbol) const;
            fix::Message buildMarketDataRequest(const std::string &_symbol, std::string &_requestId, std::string &_subscriptionType) const;
    };
}