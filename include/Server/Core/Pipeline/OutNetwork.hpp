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
            /// @param _clients List of the connected client.
            OutNetwork(MarketToNet &_input, std::vector<ClientSocket> &_clients);
            /// @brief Stop and then destroy the pipeline.
            ~OutNetwork();

            /// @brief Run the pipeline
            /// @return Return true if the pipeline as correctly started else false.
            [[nodiscard]] bool start();

            /// @brief Core function of the pipeline determining it's behavior
            void loop();

        protected:
            using ClientInfo = std::tuple<std::vector<ClientSocket>::iterator, std::string>;

            ClientInfo getClientInfo(const NetIn &_input) const;

            void logTiming(std::vector<ClientSocket>::iterator _it);


        private:
            MarketToNet &m_input;                   ///< Intput data queue.
            std::vector<ClientSocket> &m_clients;   ///< List of connected clients.

            ThreadPool<TS_SIZE_ON> m_tp;            ///< Thread pool used to send data to the target client in async.
    };
}