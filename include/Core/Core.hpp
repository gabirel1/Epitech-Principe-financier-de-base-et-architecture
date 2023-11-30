#pragma once

#include "Pipeline/Market.hpp"
#include "Pipeline/Network.hpp"

class Core
{
    public:
        Core();
        ~Core();

        void start();

        void stop();

    protected:
        void internal_start();

    private:
        bool m_running = false;

        std::vector<std::shared_ptr<net::tcp::Socket>> m_client;
        OrderBook m_ob;

        NetToSerial m_nt_to_sr;
        SerialToMarket m_sr_to_mk;
        MarketToNet m_mk_to_nt;

        pip::Network<net::tcp::Socket> m_network;
        pip::Market m_market;
};