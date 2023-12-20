#include "Message/MarketDataIncrementalRefresh.hpp"

namespace fix
{
    MarketDataIncrementalRefresh::MarketDataIncrementalRefresh()
    {
        header.setMsgType(m_msgType);
    }

    MarketDataIncrementalRefresh::~MarketDataIncrementalRefresh()
    {
    }

    void MarketDataIncrementalRefresh::set268_NoMDEntries(const std::string &_val)
    {
        m_params.emplace({ "268", _val });
    }

    void MarketDataIncrementalRefresh::set279_MDUpdateAction(const std::string &_val)
    {
        m_params.emplace({ "279", _val });
    }
}