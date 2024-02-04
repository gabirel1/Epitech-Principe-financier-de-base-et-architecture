#include "Client/Processor/Common/DataRequest.hpp"
#include "Common/Core/Logger.hpp"
#include "Common/Core/Utils.hpp"
#include "Common/Message/MarketDataSnapshotFullRefresh.hpp"
#include "Common/Message/MarketDataRequest.hpp"
#include "Common/Message/MarketDataIncrementalRefresh.hpp"

namespace proc::com
{
    template<IsBuildFIX T>
    std::optional<fix::Message> DataRequest<T>::build(char _tag, Context &_context) const
    {
        std::ignore = _context;

        if (_tag == fix::MarketDataSnapshotFullRefresh::cMsgType)
            return buildFullRefresh();
        else if (_tag == fix::MarketDataIncrementalRefresh::cMsgType)
            return buildIncrRefresh();
        return {};
    }

    template<IsBuildFIX T>
    fix::Message DataRequest<T>::buildFullRefresh() const
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

    template<IsBuildFIX T>
    fix::Message DataRequest<T>::buildIncrRefresh() const
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