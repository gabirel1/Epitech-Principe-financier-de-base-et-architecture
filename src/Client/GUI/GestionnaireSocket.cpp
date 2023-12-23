#include "Client/GUI/GestionnaireSocket.hpp"
#include <iostream>

GestionnaireSocket::GestionnaireSocket(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp)
    : thread(new ThreadSocket())
{
    m_tcp.connect(_ip, _tcp);
    m_udp.connect(_ip, _udp);

}

GestionnaireSocket::~GestionnaireSocket()
{
    thread->stop();
}

void GestionnaireSocket::sendTcpSocket(const std::string &p_data)
{
    m_tcp.send(p_data);
}

void GestionnaireSocket::startThread()
{
    thread->start();
}