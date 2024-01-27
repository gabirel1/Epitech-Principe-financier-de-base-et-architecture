#ifndef THREADSOCKET_HPP_
#define THREADSOCKET_HPP_

#include <QThread>

#include "Common/Network/Socket.hpp"
#include "Common/Core/Order.hpp"
#include "Common/Network/UDPPackage.hpp"

class ThreadSocket : public QThread {

    Q_OBJECT

    public:
        ThreadSocket(const net::Ip &_ip);
        ~ThreadSocket();

        void stop();

    protected:
        void run();

    private:
        bool m_running = false;
        net::udp::Socket m_udp;
        net::Ip m_ip;
};

#endif /* !THREADSOCKET_HPP_ */
