#include "Client/GUI/GestionnaireSocket.hpp"
#include <iostream>

GestionnaireSocket::GestionnaireSocket(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp)
    : m_thread_tcp(new ThreadSocket()),
      m_thread_udp(new ThreadSocket())
{
    m_tcp.connect(_ip, _tcp);
    m_udp.connect(_ip, _udp);
}

GestionnaireSocket::~GestionnaireSocket()
{
    m_thread_tcp->stop();
    m_thread_udp->stop();
}

void GestionnaireSocket::sendTcpSocket(const std::string &p_data)
{
    m_tcp.send(p_data);
}

void GestionnaireSocket::startThread()
{
    // m_thread_tcp->start();
    m_thread_udp->start();
}