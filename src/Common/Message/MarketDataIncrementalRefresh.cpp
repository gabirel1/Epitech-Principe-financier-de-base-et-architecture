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
        m_params.emplace({ Tag::Symbol, _val });
    }

    void MarketDataIncrementalRefresh::set110_minQty(const std::string &_val)
    {
        m_params.emplace({ Tag::MinQty, _val });
    }

    void MarketDataIncrementalRefresh::set267_noMDEntryTypes(const std::string &_val)
    {
        m_params.emplace({ Tag::NoMDEntryTypes, _val });
    }

    void MarketDataIncrementalRefresh::set269_mDEntryType(const std::string &_val)
    {
        m_params.emplace({ Tag::MDEntryType, _val });
    }

    void MarketDataIncrementalRefresh::set270_mDEntryPx(const std::string &_val)
    {
        m_params.emplace({ Tag::MDEntryPx, _val });
    }

    void MarketDataIncrementalRefresh::set279_mDUpdateAction(const std::string &_val)
    {
        m_params.emplace({ Tag::MDUpdateAction, _val });
    }
}