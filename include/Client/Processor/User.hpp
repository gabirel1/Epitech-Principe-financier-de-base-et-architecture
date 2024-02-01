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
    };
}