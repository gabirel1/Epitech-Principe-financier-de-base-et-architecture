#include "Message/MarketDataSnapshotFullRefresh.hpp"

namespace fix
{
    MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh()
    {
        header.setMsgType(m_msgType);
    }

    MarketDataSnapshotFullRefresh::~MarketDataSnapshotFullRefresh()
    {
    }

    void MarketDataSnapshotFullRefresh::set55_Symbol(const std::string &_val)
    {
        m_params.emplace({ "55", _val });
    }

    void MarketDataSnapshotFullRefresh::set268_NoMDEntries(const std::string &_val)
    {
        m_params.emplace({ "268", _val });
    }

    void MarketDataSnapshotFullRefresh::set269_MDEntryType(const std::string &_val)
    {
        m_params.emplace({ "269", _val });
    }

    void MarketDataSnapshotFullRefresh::set270_MDEntryPx(const std::string &_val)
    {
        m_params.emplace({ "270", _val });
    }
}