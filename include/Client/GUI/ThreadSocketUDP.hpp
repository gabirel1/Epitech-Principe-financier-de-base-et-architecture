#ifndef THREADSOCKETUDP_HPP_
#define THREADSOCKETUDP_HPP_

#include <QThread>
#include "Common/Network/Socket.hpp"
#include "Common/Core/Order.hpp"
#include "Common/Network/UDPPackage.hpp"

class ThreadSocketUDP : public QThread {

    Q_OBJECT

    public:
        ThreadSocketUDP(const net::Ip &_ip, uint32_t _port);
        ~ThreadSocketUDP();
        
        void stop();

    protected:
        void run();
    
    private:
        bool m_running = false;
        net::udp::Socket m_udp;
        net::Ip m_ip;
        uint32_t m_port;
};

#endif /* !THREADSOCKETUDP_HPP_ */
