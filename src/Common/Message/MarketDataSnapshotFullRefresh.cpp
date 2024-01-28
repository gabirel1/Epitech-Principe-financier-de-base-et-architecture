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
        m_params.append({ Tag::Symbol, _val });
    }

    void MarketDataSnapshotFullRefresh::set110_minQty(const std::string &_val)
    {
        m_params.append({ Tag::MinQty, _val });
    }

    void MarketDataSnapshotFullRefresh::set262_mDReqID(const std::string &_val)
    {
        m_params.append({ Tag::MDReqID, _val });
    }

    void MarketDataSnapshotFullRefresh::set267_noMDEntryTypes(const std::string &_val)
    {
        m_params.append({ Tag::NoMDEntryTypes, _val });
    }

    void MarketDataSnapshotFullRefresh::set269_mDEntryType(const std::string &_val)
    {
        m_params.append({ Tag::MDEntryType, _val });
    }

    void MarketDataSnapshotFullRefresh::set270_mDEntryPx(const std::string &_val)
    {
        m_params.append({ Tag::MDEntryPx, _val });
    }

    MarketDataSnapshotFullRefresh &MarketDataSnapshotFullRefresh::operator+=(MarketDataSnapshotFullRefresh _ref)
    {
        m_params[Tag::MinQty] += "," + _ref.m_params[Tag::MinQty];
        m_params[Tag::NoMDEntryTypes] = std::to_string(utils::to<size_t>(m_params[Tag::NoMDEntryTypes]) + utils::to<size_t>(_ref.m_params[Tag::NoMDEntryTypes]));
        m_params[Tag::MDEntryType] += "," + _ref.m_params[Tag::MDEntryType];
        m_params[Tag::MDEntryPx] += "," + _ref.m_params[Tag::MDEntryPx];
        return *this;
    }
}