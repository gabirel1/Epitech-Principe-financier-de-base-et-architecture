#include "Client/GUI/GestionnaireSocket.hpp"
#include <iostream>

GestionnaireSocket::GestionnaireSocket(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp, NetIO &_netio)
    : m_netio(_netio), m_udp_handler(_udp, _ip, m_netio.udp_in, m_netio.udp_out), m_tcp_handler(_ip, _tcp, m_netio.tcp_in, m_netio.tcp_out)
{
    start();
}

GestionnaireSocket::~GestionnaireSocket()
{
    (void)m_udp_handler.stop();
}

void GestionnaireSocket::start()
{
    m_udp_handler.start();
    m_tcp_handler.start();
}