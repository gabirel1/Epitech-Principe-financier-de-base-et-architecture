#pragma once

#include "Server/Core/Pipeline/Core.hpp"
#include "Server/Core/Pipeline/Naming.hpp"
#include "Server/Core/OrderBook.hpp"

namespace pip
{
    class OBEvent : public Pipeline<OBEvent>
    {
        public:
            /// @brief Core pipeline type
            using PipeType = Pipeline<OBEvent>;

            OBEvent(OrderBook::EventQueue &_input, UdpInput &_udp, RawOutput &_tcp);
            ~OBEvent();

            /// @brief Run the pipeline
            /// @return Return true if the pipeline as correctly started else false.
            [[nodiscard]] bool start();

            /// @brief Core function of the pipeline determining it's behavior
            void loop();

        protected:
            bool createTcp(OrderBook::Event &input);
            bool createUdp(OrderBook::Event &input);

        private:
            OrderBook::EventQueue &m_input;
            UdpInput &m_udp;
            RawOutput &m_tcp;

            ThreadPool m_tp;
    };
}