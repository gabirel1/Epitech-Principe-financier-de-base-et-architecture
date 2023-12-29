#include <iostream>

#include "Common/Core/Logger.hpp"
#include "Server/Core/Core.hpp"

Core::Core(uint32_t _tcp_port, uint32_t _udp_port)
    : m_ob(), m_innet(m_client, m_nt_to_sr, _tcp_port),
        m_action(m_nt_to_sr, m_sr_to_mk, m_mk_to_nt),
        m_market(m_ob, m_sr_to_mk, m_mk_to_nt),
        m_outnet(m_mk_to_nt)
{
}

Core::~Core()
{
    stop();
}

void Core::start()
{
    Logger::Log("[Core] Starting...");
    m_running = true;
    if (!internal_start())
        return;
    while (m_running)
    {
        try {
            m_tcp_innet.status();
            m_market.status();
            m_action.status();
            m_outnet.status();
        } catch (std::future_error &_e) {
            Logger::Log("[Core] Pipeline have crash: ", _e.what(), "\n\t> with the code: ", _e.code());
            stop();
        }
    }
    stop();
}

void Core::stop()
{
    if (m_running) {
        m_running = false;
        Logger::Log("[Core] Stoping...");
        while (m_tcp_innet.stop() != std::future_status::deferred)
        Logger::Log("[Core] Input network exited");
        while (m_action.stop() != std::future_status::deferred)
        Logger::Log("[Core] Action pipeline exited");
        while (m_market.stop() != std::future_status::deferred)
        Logger::Log("[Core] Market exited");
        while (m_outnet.stop() != std::future_status::deferred)
        Logger::Log("[Core] Output network exited");
        Logger::Log("[Core] All pipeline are stoped");
    }
}

bool Core::internal_start()
{
    Logger::Log("[Core] Starting pipeline...");
    if (!m_outnet.start()) {
        Logger::Log("[Core] Failed to start output network");
        stop();
    } else if (!m_market.start()) {
        Logger::Log("[Core] Failed to start market");
        stop();
    } else if (!m_action.start()) {
        Logger::Log("[Core] Failed to start action pipeline");
        stop();
    } else if (!m_tcp_innet.start()) {
        Logger::Log("[Core] Failed to start input network");
        stop();
    } else {
        Logger::Log("[Core] All pipeline are running");
        return true;
    }
    return false;
}