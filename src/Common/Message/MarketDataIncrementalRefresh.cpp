#include "Common/Message/MarketDataIncrementalRefresh.hpp"
#include "Common/Message/Tag.hpp"

namespace fix
{
    MarketDataIncrementalRefresh::MarketDataIncrementalRefresh()
    {
        header.set35_MsgType(MsgType);
    }

    void MarketDataIncrementalRefresh::set55_symbol(const std::string &_val)
    {
        m_params.append({ Tag::Symbol, _val });
    }

    void MarketDataIncrementalRefresh::set110_minQty(const std::string &_val)
    {
        m_params.append({ Tag::MinQty, _val });
    }

    void MarketDataIncrementalRefresh::set267_noMDEntryTypes(const std::string &_val)
    {
        m_params.append({ Tag::NoMDEntryTypes, _val });
    }

    void MarketDataIncrementalRefresh::set269_mDEntryType(const std::string &_val)
    {
        m_params.append({ Tag::MDEntryType, _val });
    }

    void MarketDataIncrementalRefresh::set270_mDEntryPx(const std::string &_val)
    {
        m_params.append({ Tag::MDEntryPx, _val });
    }

    void MarketDataIncrementalRefresh::set279_mDUpdateAction(const std::string &_val)
    {
        m_params.append({ Tag::MDUpdateAction, _val });
    }

    void MarketDataIncrementalRefresh::set280_mDEntryRefID(const std::string &_val)
    {
        m_params.append({ Tag::MDentryRefID, _val });
    }

    MarketDataIncrementalRefresh &MarketDataIncrementalRefresh::operator+=(MarketDataIncrementalRefresh _ref)
    {
        m_params[Tag::Symbol] += _ref.m_params[Tag::Symbol];
        m_params[Tag::MinQty] += _ref.m_params[Tag::MinQty];
        m_params[Tag::NoMDEntryTypes] = std::to_string(utils::to<size_t>(m_params[Tag::NoMDEntryTypes]) + utils::to<size_t>(_ref.m_params[Tag::NoMDEntryTypes]));
        m_params[Tag::MDEntryType] += _ref.m_params[Tag::MDEntryType];
        m_params[Tag::MDEntryPx] += _ref.m_params[Tag::MDEntryPx];
        m_params[Tag::MDUpdateAction] += _ref.m_params[Tag::MDUpdateAction];
        return *this;
    }

}