#pragma once

#include "Common/Thread/Pool.hpp"
#include "Server/Core/Pipeline/Core.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

#ifndef TS_SIZE_OB
    #define TS_SIZE_OB 1
#endif

namespace pip
{
    /// @brief Pipeline managing the OrderBook.
    class Market : public Pipeline<Market>
    {
        public:
            /// @brief Construct the pipeline.
            /// @param _ob OrderBook reference.
            /// @param _input Input data queue.
            /// @param _output Output data queue.
            Market(OrderBook &_ob, SerialToMarket &_input, MarketToNet &_output);
            /// @brief Stop the pipeline and the destroy it.
            ~Market();

            /// @brief Run the pipeline
            /// @return Return true if the pipeline as correctly started else false.
            [[nodiscard]] bool start();

            /// @brief Core function of the pipeline determining it's behavior.
            void loop();

        private:
            /// @brief Apply the action received by the pip::Action pipeline on the OrderBook.
            /// @param _data Data to build and run action on the OrderBook.
            void process(MarketIn &_data);

            /// @brief Build the reply for udp and tcp of the OrderBook::Data::Action::Add process.
            /// @param _data Data receive and modify by the function OrderBook::add.
            /// @param _result Result of the actions.
            void buildAdd(const MarketIn &_data, Quantity _quantity, bool _result);

            SerialToMarket &m_input;        ///< Intput data queue.
            MarketToNet &m_output;          ///< Output data queue.

            ThreadPool<TS_SIZE_OB> m_tp;    ///< Thread pool to run async processing.

            OrderBook &m_ob;                ///< OrderBook manage by the pipeline.
    };
}