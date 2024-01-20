#include "Server/Core/MarketContainer.hpp"

MarketContainer::MarketContainer(const std::string &_name, UdpInput &_udp, RawOutput &_tcp);
    : m_name(_name), m_ob(m_q_event),
        m_market(m_name, m_ob, m_q_action, m_q_event),
        m_obevent(m_name, m_q_event, _udp, _tcp)

{
}

MarketContainer::~MarketContainer()
{
    stop();
}

bool MarketContainer::start()
{
    if (m_market.start()) {
        if (m_obevent.start())
            return true;
        m_market.stop();
    }
    return false;
}

ThreadStatus MarketContainer::stop()
{
    return ThreadStatus(m_market.status(), m_obevent.status());
}

bool MarketContainer::status(float _to)
{
    return m_market.status() && m_obevent.status();
}
