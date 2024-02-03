#pragma once

#include "Client/Processor/DataRequest.hpp"

namespace proc
{
    class OBData : public com::DataRequest, public IEntry
    {
        public:
            OBData() = default;
            ~OBData() = default;

            virtual std::optional<fix::Message> process(const std::string &_entry, Context &_ctx) override final;

        private:
            fix::MarketDataRequest buildForRefresh();
            fix::MarketDataRequest buildForSub();
            fix::MarketDataRequest buildForUnsub();

            bool setDepth(const std::string &_depth, fix::MarketDataRequest &_request);
            bool setSide(const std::string &_depth, fix::MarketDataRequest &_request);
    };
}