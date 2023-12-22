#pragma once

#include "Common/Thread/Pool.hpp"
#include "Server/Core/Pipeline/Core.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

#ifndef TS_SIZE_ON
    #define TS_SIZE_ON 1
#endif

namespace pip
{
    class OutNetwork : public Pipeline<OutNetwork>
    {
        public:

            using PipeType = Pipeline<OutNetwork>;

            OutNetwork(MarketToNet &_input);
            ~OutNetwork();

            [[nodiscard]] bool start();
            bool status(float _to = 1.f);

            void loop();

        private:
            MarketToNet &m_input;

            ThreadPool<TS_SIZE_ON> m_tp;
    };
}