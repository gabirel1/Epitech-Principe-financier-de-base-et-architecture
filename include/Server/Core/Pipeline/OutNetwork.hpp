#pragma once

#include "Common/Thread/Pool.hpp"
#include "Server/Core/Pipeline/Core.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

#ifndef TS_SIZE_ON
    #define TS_SIZE_ON 1
#endif

namespace pip
{
    /// @brief Pipeline to send reply to a client over TCP network.
    class OutNetwork : public Pipeline<OutNetwork>
    {
        public:
            /// @brief Core pipeline type.
            using PipeType = Pipeline<OutNetwork>;

            /// @brief Construct the pipeline.
            /// @param _input Input data queue.
            OutNetwork(MarketToNet &_input);
            /// @brief Stop and then destroy the pipeline.
            ~OutNetwork();

            /// @brief Core function of the pipeline determining it's behavior
            void loop();

        private:
            MarketToNet &m_input;           ///< Intput data queue.

            ThreadPool<TS_SIZE_ON> m_tp;    ///< Thread pool used to send data to the target client in async.
    };
}