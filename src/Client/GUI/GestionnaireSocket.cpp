#include "Client/GUI/GestionnaireSocket.hpp"
#include <iostream>

GestionnaireSocket::GestionnaireSocket(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp)
    : m_thread_udp(nullptr)
{
    std::cout << "GestionnaireSOcket => " << _udp <<std::endl;
    m_tcp.connect(_ip, _tcp);
    std::cout << "Port TCP => " << m_tcp.getPort() <<std::endl;

    m_thread_udp = new ThreadSocket(_ip);
    startThread();
}

GestionnaireSocket::~GestionnaireSocket()
{
    // m_thread_tcp->stop();
    m_thread_udp->stop();
}

void GestionnaireSocket::sendTcpSocket(const std::string &p_data)
{
    m_tcp.send(p_data);
}

void GestionnaireSocket::startThread()
{
    std::cout << "START m_thread_udp" << std::endl;
    m_thread_udp->start();
}