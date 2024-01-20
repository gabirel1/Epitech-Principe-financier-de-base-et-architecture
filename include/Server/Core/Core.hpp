#pragma once

#include "Server/Core/Pipeline/Action.hpp"
#include "Server/Core/Pipeline/InNetwork.hpp"
#include "Server/Core/Pipeline/OutNetwork.hpp"
#include "Server/Core/Pipeline/UDPOutNetwork.hpp"
#include "Server/Core/MarketContainer.hpp"
#include "Server/Network/Processor.hpp"

class Core
{
    public:
        Core(uint32_t _tcp_port, uint32_t _udp_port);
        ~Core();

        [[nodiscard]] bool start();

        void stop();

    protected:
        bool internal_start();
        void market_init();

    private:
        bool m_running = false;

        std::vector<ClientSocket> m_clients;

        UdpInput m_q_udp;
        NetToAction m_q_action;
        MarketEntry m_q_markets;
        MarketToNet m_q_tcp;

        pip::InNetwork<net::tcp::Socket, &net::tcp::processor, ClientSocket> m_innet;
        pip::Action m_action;
        std::map<std::string, MarketContainer> m_markets;
        pip::OutNetwork m_outnet;
        pip::UDPOutNetwork m_udp;
};