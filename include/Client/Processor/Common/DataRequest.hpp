#pragma once

#include "Client/Processor/IProcessor.hpp"

template<class T>
concept IsBuildFIX = std::is_base_of_v<proc::IBuildFIX, T>;

namespace proc::com
{
    template<IsBuildFIX T>
    class DataRequest : public T
    {
        public:
            virtual std::optional<fix::Message> build(char _tag, Context &_context) const override final;

        private:
            fix::Message buildFullRefresh() const;
            fix::Message buildIncrRefresh() const;
    };
}

#include "Client/Processor/Common/DataRequest.inl"