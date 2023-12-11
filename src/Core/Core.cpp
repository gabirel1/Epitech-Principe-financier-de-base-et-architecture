#include <iostream>

#include "Core/Core.hpp"

Core::Core(uint32_t _tcp_port, uint32_t _udp_port)
    : m_ob(), m_innet(m_client, m_nt_to_sr, _tcp_port), m_market(m_ob, m_sr_to_mk, m_mk_to_nt)
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
        try {
            m_innet.status();
            m_market.status();
        } catch (std::future_error &_e) {
            std::cout << "exception: " << _e.what() << " | " << _e.code() << std::endl;
            m_running = false;
        }
    }
    stop();
}

void Core::stop()
{
    if (m_running) {
        m_running = false;
        m_market.stop();
        m_innet.stop();
    }
}

void Core::internal_start()
{
    m_market.start();
    m_innet.start();
}