#pragma once

#include "Core/Pipeline/Core.hpp"
#include "Core/Pipeline/Naming.hpp"

namespace pip
{
    class OutNetwork : public Pipeline<OutNetwork>
    {
        public:

            using PipeType = Pipeline<OutNetwork>;

            OutNetwork(MarketToNet &_input);
            ~OutNetwork();

            [[nodiscard]] bool start();
            void status(float _to = 1.f);

            void loop();

        private:
            MarketToNet &m_input;
    };
}