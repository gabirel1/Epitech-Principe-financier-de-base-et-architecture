#pragma once

#include "Common/Network/Socket.hpp"

using UDPOutput = ts::Queue<data::UDPPackage>;
using UDPInput = ts::Queue<data::UDPPackage>;

class UDPHandler
{
    public:
        UDPHandler(uint32_t _port, const net::Ip &_ip, UDPInput &_input, UDPOutput &_output);
        ~UDPHandler();

        void start();

        void stop();

        [[nodiscard]] uint32_t port() const;
        [[nodiscard]] Ip port() const;

    protected:
        void loop();

    private:
        bool m_running = false;
        net::Ip m_ip;

        UDPInput &m_input;
        UDPOutput &m_output;

        std::shared_ptr<net::udp::Socket> m_socket;
        net::Selector<net::udp::Socket> m_selector;

        std::thread m_thread;
}