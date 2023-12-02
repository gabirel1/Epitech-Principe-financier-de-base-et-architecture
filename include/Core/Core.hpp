#pragma once

#include "Pipeline/Market.hpp"

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

        OrderBook m_ob;

        SerialToMarket m_sr_to_mk;
        MarketToNet m_mk_to_nt;

        pip::Market m_market;
};