#ifndef THREADSOCKET_HPP_
#define THREADSOCKET_HPP_

#include <QThread>
#include "../../Common/Network/Socket.hpp"
#include "../../Common/Core/Order.hpp"

struct UDPPackage
{
    uint32_t time;
    uint64_t id;
    uint8_t flag;
    Quantity quantity;
    Price price;
};

class ThreadSocket : public QThread {

    Q_OBJECT

    public:
        ThreadSocket();
        ~ThreadSocket();
        
        void stop();

    protected:
        void run();
    
    private:
        bool m_running = false;
        net::udp::Socket m_udp;
};

#endif /* !THREADSOCKET_HPP_ */
