#pragma once

#include <memory>
#include <vector>

#include "Client/Data/IProcessor.hpp"
#include "Common/Network/Ip.hpp"
#include "Common/Thread/Queue.hpp"
#include "Client/Data/UDPHandler.hpp"
#include "Client/Data/UserInput.hpp"

using TCPInput = ts::Queue<fix::Message>;
using TCPOutput = ts::Queue<fix::Serializer::AnonMessage>;

class Core
{
    public:
        Core(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp);
        ~Core();

        void start();

        void stop();

    private:
        std::vector<std::shared_ptr<proc::IMessage>> m_proc_tcp;
        std::vector<std::shared_ptr<proc::IUDP>> m_proc_udp;
        std::vector<std::shared_ptr<proc::IEntry>> m_proc_entry;

        bool m_running = false;

        TCPInput m_tcp_in;
        TCPOutput m_tcp_out;
        UDPInput m_udp_in;
        UDPOutput m_udp_out;
        EntryOutput m_entry;

        UserInput m_input;
        UDPHandler m_udp;

        std::thread m_thread;
};