#include "Client/Core/Core.hpp"

Core::Core(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp)
{
    // m_tcp.connect(_ip, _tcp);
    // m_udp.connect(_ip, _udp);
    m_tcp.send("this is a test");
}

Core::~Core()
{
    stop();
}

void Core::start()
{
    m_running = true;
    while (m_running) {}
}

void Core::stop()
{
    m_running = false;
}