#include "Client/GUI/ThreadSocketUDP.hpp"
#include <iostream>

ThreadSocketUDP::ThreadSocketUDP(const net::Ip &_ip, uint32_t _port)
    :m_ip(_ip),
     m_port(_port)
{
    m_running = true;
}

ThreadSocketUDP::~ThreadSocketUDP()
{
}

void ThreadSocketUDP::run()
{
    int error = 0;

    m_udp.broadcastOn(8081);
    m_udp.bind();

    while (m_running) {

        std::cout << "Size UDPPackage = " << sizeof(data::UDPPackage) <<std::endl;

        std::cout << m_udp.receiveUDP(sizeof(data::UDPPackage), error) <<std::endl;
    }
}

void ThreadSocketUDP::stop()
{
    m_running = false;
}