#pragma once

#include "Common/Network/Socket.hpp"

class Core
{
    public:
        Core(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp);
        ~Core();

        void start();

        void stop();

    private:
        bool m_running = false;

        net::tcp::Socket m_tcp;
};