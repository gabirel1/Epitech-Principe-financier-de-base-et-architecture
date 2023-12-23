#include "Client/Core/Core.hpp"
#include "Server/Core/OrderBook.hpp"
#include <iostream>
#include "Common/Message/Logon.hpp"

Core::Core(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp)
{
    m_tcp.connect(_ip, _tcp);
    m_udp.connect(_ip, _udp);

    

    m_running = true;
}

Core::~Core()
{
    stop();
}

void Core::start()
{
    int error = 0;

    while (m_running) {
        // std::cout << m_running << std::endl;
    }
}

void Core::stop()
{
    m_running = false;
}