#pragma once

#include "Common/Thread/Pool.hpp"
#include "Server/Core/Pipeline/Core.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

#ifndef TS_SIZE_OB
    #define TS_SIZE_OB 1
#endif

namespace pip
{
    class Market : public Pipeline<Market>
    {
        public:
            Market(OrderBook &_ob, SerialToMarket &_input, MarketToNet &_output);
            ~Market();

            [[nodiscard]] bool start();
            bool status(float _to = 1.f);

            void loop();

        private:
            void process(MarketIn &_data);

            void send(const MarketIn _data);

            SerialToMarket &m_input;
            MarketToNet &m_output;

            ThreadPool<TS_SIZE_OB> m_tp;

            OrderBook &m_ob;
    };
}