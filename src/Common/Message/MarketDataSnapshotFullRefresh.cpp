#include "Common/Message/MarketDataSnapshotFullRefresh.hpp"
#include "Common/Message/Tag.hpp"

namespace fix
{
    MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh()
    {
        header.set35_MsgType(MsgType);
    }

    void MarketDataSnapshotFullRefresh::set55_symbol(const std::string &_val)
    {
        m_params.emplace({ Tag::Symbol, _val });
    }

    void MarketDataSnapshotFullRefresh::set110_minQty(const std::string &_val)
    {
        m_params.emplace({ Tag::MinQty, _val });
    }

    void MarketDataSnapshotFullRefresh::set267_noMDEntryTypes(const std::string &_val)
    {
        m_params.emplace({ Tag::NoMDEntryTypes, _val });
    }

    void MarketDataSnapshotFullRefresh::set269_mDEntryType(const std::string &_val)
    {
        m_params.emplace({ Tag::MDEntryType, _val });
    }

    void MarketDataSnapshotFullRefresh::set270_mDEntryPx(const std::string &_val)
    {
        m_params.emplace({ Tag::MDEntryPx, _val });
    }
}