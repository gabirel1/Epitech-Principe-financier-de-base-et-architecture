#pragma once

#include "Server/Core/Pipeline/Action.hpp"
#include "Server/Core/Pipeline/InNetwork.hpp"
#include "Server/Core/Pipeline/Market.hpp"
#include "Server/Core/Pipeline/OutNetwork.hpp"
#include "Server/Core/Pipeline/UDPOutNetwork.hpp"
#include "Server/Network/Processor.hpp"

class Core
{
    public:
        Core(uint32_t _tcp_port, uint32_t _udp_port);
        ~Core();

        void start();

        void stop();

    protected:
        bool internal_start();

    private:
        bool m_running = false;

        std::vector<ClientSocket> m_tcp_client;
        std::vector<std::shared_ptr<net::udp::Socket>> m_udp_client;
        OrderBook m_ob;

        UdpInput m_udp_input;
        NetToSerial m_nt_to_sr;
        SerialToMarket m_sr_to_mk;
        MarketToNet m_mk_to_nt;

        pip::InNetwork<net::tcp::Socket, &net::tcp::processor, ClientSocket> m_innet;
        pip::Action m_action;
        pip::Market m_market;
        pip::OutNetwork m_outnet;
        pip::UDPOutNetwork m_udp;
};