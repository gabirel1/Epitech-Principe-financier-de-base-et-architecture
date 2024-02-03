#include "Client/Processor/Common/DataRequest.hpp"

namespace proc::com
{
    std::optional<fix::Message> DataRequest::build(char _tag, Context &_context) const
    {
        std::ignore = _context;

        if (_tag == fix::MarketDataSnapshotFullRefresh::cMsgType)
            return buildFullRefresh();
        else if (_tag == fix::MarketDataSnapshotFullRefresh::cMsgType)
            return buildIncrRefresh();
        return {};
    }

    fix::Message OrderBook::buildFullRefresh() const
    {
        fix::MarketDataRequest request;
        const std::vector<std::string> symbols{ MARKET_NAME };
        std::string lsymbols;

        request.set146_noRelatedSym(std::to_string(symbols.size()));
        for (const auto &_sym : symbols)
            lsymbols += _sym + ",";
        if (!symbols.empty())
            lsymbols.erase(lsymbols.end());
        request.set55_symbol(lsymbols);
        request.set262_mDReqID(utils::id());
        request.set263_subscriptionRequestType("0");
        request.set264_marketDepth("0");
        request.set267_noMDEntryTypes("2");
        request.set269_mDEntryType("0,1");
        Logger::Log("[TCPInput] Sending a data full refresh request");
        return request;
    }

    fix::Message OrderBook::buildIncrRefresh() const
    {
        fix::MarketDataRequest request;
        const std::vector<std::string> symbols{ MARKET_NAME };
        std::string lsymbols;

        request.set146_noRelatedSym(std::to_string(symbols.size()));
        for (const auto &_sym : symbols)
            lsymbols += _sym + ",";
        if (!symbols.empty())
            lsymbols.erase(lsymbols.end());
        request.set55_symbol(lsymbols);
        request.set262_mDReqID(utils::id());
        request.set263_subscriptionRequestType("1");
        request.set264_marketDepth("0");
        request.set267_noMDEntryTypes("2");
        request.set269_mDEntryType("0,1");
        Logger::Log("[TCPInput] Sending a data incremental refresh request");
        return request;
    }
}