#pragma once

#include "Client/Processor/Common/DataRequest.hpp"
#include "Common/Message/MarketDataRequest.hpp"

namespace proc
{
    class OBData : public com::DataRequest<IEntry>
    {
        public:
            OBData() = default;
            ~OBData() = default;

            virtual std::optional<fix::Message> process(const std::string &_entry, Context &_ctx) override final;

        private:
            std::optional<fix::MarketDataRequest> buildRequest(const std::vector<const char *> &_words);
            std::optional<fix::Message> buildForUnsub(const std::vector<const char *> &_words);

            bool applyModification(char _param, const std::string &_value, fix::MarketDataRequest &_refresh);

            bool setSymbol(const std::string &_symbol, fix::MarketDataRequest &_request);
            bool setDepth(const std::string &_depth, fix::MarketDataRequest &_request);
            bool setSide(const std::string &_side, fix::MarketDataRequest &_request);
            bool setReqId(const std::string &_reqid, fix::MarketDataRequest &_request);
    };
}