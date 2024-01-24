#ifndef GESTIONNAIRESOCKET_HPP_
#define GESTIONNAIRESOCKET_HPP_

#include "Common/Network/Socket.hpp"
#include "ThreadSocket.hpp"

class GestionnaireSocket
{
    public:
        GestionnaireSocket(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp);
        ~GestionnaireSocket();

        void sendTcpSocket(const std::string &p_data);
        void sendUdpSocket(const std::string &p_data);

        void startThread();
    protected:
    private:

        net::tcp::Socket m_tcp;
        net::udp::Socket m_udp;

        // ThreadSocket *m_thread_tcp;
        ThreadSocket *m_thread_udp;
};

#endif /* !GESTIONNAIRESOCKET_HPP_ */
