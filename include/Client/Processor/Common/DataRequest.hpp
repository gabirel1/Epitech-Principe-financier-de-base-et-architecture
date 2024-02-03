#pragma once

#include "Client/Processor/IProcessor.hpp"

namespace proc::com
{
    class DataRequest : public proc::IBuildFIX
    {
        public:
            std::optional<fix::Message> build(char _tag, Context &_context) const;

        private:
            fix::Message buildFullRefresh() const;
            fix::Message buildIncrRefresh() const;

    };
}