#pragma once

#include "Core/Pipeline/Core.hpp"
#include "Core/Pipeline/Naming.hpp"
#include "Thread/Pool.hpp"

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
            void status(float _to = 1.f);

            void loop();

        private:
            void process(MarketIn _data);

            void send(MarketIn _data);

            SerialToMarket &m_input;
            MarketToNet &m_output;

            ThreadPool<TS_SIZE_OB> m_tp;

            OrderBook &m_ob;
    };
}