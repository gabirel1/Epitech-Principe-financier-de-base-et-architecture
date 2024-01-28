#ifndef THREADSOCKETTCP_HPP_
#define THREADSOCKETTCP_HPP_

#include <QThread>

#include "Common/Network/Selector.hpp"
#include "Common/Message/Serializer.hpp"
#include "Common/Message/Fix.hpp"

using TCPOutput = ts::Queue<fix::Serializer::AnonMessage>;
using TCPInput = ts::Queue<fix::Message>;

class ThreadSocketTCP : public QThread
{
    Q_OBJECT
    public:

        using Client = std::shared_ptr<net::tcp::Socket>;

        ThreadSocketTCP(const net::Ip &_ip, uint32_t _port, TCPInput &m_input, TCPOutput &_output);
        ~ThreadSocketTCP();

        void stop();

    protected:
        void run();

    private:

        bool m_running = false;

        TCPInput &m_input;
        TCPOutput &m_output;

        std::shared_ptr<net::tcp::Socket> m_tcp;
        net::Selector<net::tcp::Socket> m_select;

        net::Ip m_ip;
        uint32_t m_port;

        std::vector<Client> m_clients;
};

#endif /* !THREADSOCKETTCP_HPP_ */
