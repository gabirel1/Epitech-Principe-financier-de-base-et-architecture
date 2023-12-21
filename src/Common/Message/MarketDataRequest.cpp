#include "Message/MarketDataRequest.hpp"

namespace fix
{
    MarketDataRequest::MarketDataRequest()
    {
        header.setMsgType(m_msgType);
    }

    MarketDataRequest::~MarketDataRequest()
    {
    }

    void MarketDataRequest::set262_MDReqID(const std::string &_val)
    {
        m_params.emplace({ "262", _val });
    }

    void MarketDataRequest::set263_SubscriptionRequestType(const std::string &_val)
    {
        m_params.emplace({ "263", _val });
    }

    void MarketDataRequest::set264_MarketDepth(const std::string &_val)
    {
        m_params.emplace({ "264", _val });
    }

    void MarketDataRequest::set267_NoMDEntryTypes(const std::string &_val)
    {
        m_params.emplace({ "267", _val });
    }

    void MarketDataRequest::set269_MDEntryType(const std::string &_val)
    {
        m_params.emplace({ "269", _val });
    }

    void MarketDataRequest::set146_NoRelatedSym(const std::string &_val)
    {
        m_params.emplace({ "146", _val });
    }

    void MarketDataRequest::set55_Symbol(const std::string &_val)
    {
        m_params.emplace({ "55", _val });
    }
}