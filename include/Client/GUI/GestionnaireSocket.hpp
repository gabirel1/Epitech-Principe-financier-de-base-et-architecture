#ifndef GESTIONNAIRESOCKET_HPP_
#define GESTIONNAIRESOCKET_HPP_

#include "Client/Data/OderBook.hpp"
#include "Client/Data/UDPHandler.hpp"
#include "Client/GUI/ThreadSocketTCP.hpp"
#include "Common/Network/Socket.hpp"

struct NetIO
{
    TCPInput tcp_in;
    TCPOutput tcp_out;
    UDPInput udp_in;
    UDPOutput udp_out;
};

class GestionnaireSocket
{
    public:
        GestionnaireSocket(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp, NetIO &);
        ~GestionnaireSocket();

        void start();

    private:
        NetIO &m_netio;

        UDPHandler m_udp_handler;
        ThreadSocketTCP m_tcp_handler;
};

#endif /* !GESTIONNAIRESOCKET_HPP_ */
