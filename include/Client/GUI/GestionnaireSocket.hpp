#ifndef GESTIONNAIRESOCKET_HPP_
#define GESTIONNAIRESOCKET_HPP_

#include "Client/Data/OderBook.hpp"
#include "Client/Data/UDPHandler.hpp"
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

        void sendTcpSocket(const std::string &p_data);
        void sendUdpSocket(const std::string &p_data);

        void start();

    private:
        NetIO &m_netio;

        // net::tcp::Socket m_tcp;
        UDPHandler m_udp_handler;
};

#endif /* !GESTIONNAIRESOCKET_HPP_ */
