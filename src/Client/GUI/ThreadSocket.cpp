#include "Client/GUI/ThreadSocket.hpp"
#include <iostream>

ThreadSocket::ThreadSocket(net::udp::Socket p_udp)
    : m_udp(p_udp)
{
    m_running = true;
}

ThreadSocket::~ThreadSocket()
{
}

void ThreadSocket::run()
{
    int error = 0;
    size_t size = 50;

    while (m_running) {
        m_udp.receive(sizeof(UDPPackage), error);
        std::cout << "thread UDP" << std::endl; 
    }
}

void ThreadSocket::stop()
{
    m_running = false;
}