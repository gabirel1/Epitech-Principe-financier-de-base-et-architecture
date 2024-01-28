#include "Client/GUI/GestionnaireSocket.hpp"
#include <iostream>

GestionnaireSocket::GestionnaireSocket(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp, NetIO &_netio)
    : m_netio(_netio), m_udp_handler(_udp, _ip, m_netio.udp_in, m_netio.udp_out)
{
    start();
}

GestionnaireSocket::~GestionnaireSocket()
{
    m_udp_handler.stop();
}

void GestionnaireSocket::sendTcpSocket(const std::string &p_data)
{
    // m_tcp.send(p_data);
}

void GestionnaireSocket::start()
{
    m_udp_handler.start();
}