#include "Server/Core/MarketContainer.hpp"

MarketContainer::MarketContainer(const std::string &_name, UdpInput &_udp, RawOutput &_tcp)
    : m_name(_name), m_ob(m_name, m_q_event),
        m_market(m_name, m_ob, m_q_action, _tcp),
        m_obevent(m_name, m_q_event, _udp, _tcp)

{
}

MarketContainer::~MarketContainer()
{
    (void)stop();
}

bool MarketContainer::start()
{
    if (m_market.start()) {
        if (m_obevent.start())
            return true;
        (void)m_market.stop();
    }
    return false;
}

MarketContainer::ThreadStatus MarketContainer::stop()
{
    return ThreadStatus(m_market.stop(), m_obevent.stop());
}

bool MarketContainer::status(float _to)
{
    return m_market.status(_to) && m_obevent.status(_to);
}

const std::string &MarketContainer::getName() const
{
    return m_name;
}

ActionToMarket &MarketContainer::getInput()
{
    return m_q_action;
}