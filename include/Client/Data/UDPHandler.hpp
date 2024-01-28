#pragma once

#include <memory>

#include "Common/Network/Selector.hpp"
#include "Common/Network/Socket.hpp"
#include "Common/Network/UDPPackage.hpp"
#include "Common/Thread/Queue.hpp"

using UDPOutput = ts::Queue<data::UDPPackage>;
using UDPInput = ts::Queue<data::UDPPackage>;

class UDPHandler
{
    public:
        using Client = net::Selector<net::udp::Socket>::Client;

        UDPHandler(uint32_t _port, const net::Ip &_ip, UDPInput &_input, UDPOutput &_output);
        ~UDPHandler();

        [[nodiscard]] bool start();

        [[nodiscard]] bool stop();

        [[nodiscard]] uint32_t port() const;
        [[nodiscard]] net::Ip ip() const;

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
};