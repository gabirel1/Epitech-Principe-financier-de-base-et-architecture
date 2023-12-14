#pragma once

#include "Core/Pipeline/Action.hpp"
#include "Core/Pipeline/InNetwork.hpp"
#include "Core/Pipeline/Market.hpp"
#include "Core/Pipeline/OutNetwork.hpp"

class Core
{
    public:
        Core(uint32_t _tcp_port, uint32_t _udp_port);
        ~Core();

        void start();

        void stop();

    protected:
        void internal_start();

    private:
        bool m_running = false;

        std::vector<ClientSocket> m_client;
        OrderBook m_ob;

        NetToSerial m_nt_to_sr;
        SerialToMarket m_sr_to_mk;
        MarketToNet m_mk_to_nt;

        pip::InNetwork<net::tcp::Socket> m_innet;
        pip::Action m_action;
        pip::Market m_market;
        pip::OutNetwork m_outnet;
};