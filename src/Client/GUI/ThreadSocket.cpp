#include "Client/GUI/ThreadSocket.hpp"
#include <iostream>

ThreadSocket::ThreadSocket(const net::Ip &_ip)
    :m_ip(_ip)
{
    m_running = true;
}

ThreadSocket::~ThreadSocket()
{
}

void ThreadSocket::run()
{
    int error = 0;

    m_udp.broadcastOn(8081);
    m_udp.bind();

    while (m_running) {

        std::cout << "Size UDPPackage = " << sizeof(data::UDPPackage) <<std::endl;

        std::cout << m_udp.receiveUDP(sizeof(data::UDPPackage), error) <<std::endl;
        // UDPPackage* udp = const_cast<UDPPackage*>(reinterpret_cast<const UDPPackage*>(&pack));
        // std::cout << udp->id << " || "<< udp->price << " || "<< udp->quantity << " || "<< udp->time << std::endl;
    }
}

void ThreadSocket::stop()
{
    m_running = false;
}