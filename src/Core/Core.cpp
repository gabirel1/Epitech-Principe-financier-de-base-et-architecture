#include "Core/Core.hpp"

Core::Core()
    : m_ob(), m_market(m_ob, m_sr_to_mk, m_mk_to_nt)
{
}

Core::~Core()
{
    stop();
}

void Core::start()
{
    m_running = true;
    internal_start();
    while (m_running)
    {
        m_market.status();
    }
    stop();
}

void Core::stop()
{
    if (m_running) {
        m_running = false;
        m_market.stop();
    }
}

void Core::internal_start()
{
    m_market.start();
}