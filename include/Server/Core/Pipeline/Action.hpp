#pragma once

#include "Server/Core/Pipeline/Core.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

namespace pip
{
    /// @brief Pipeline running action depending on received message.
    class Action : public Pipeline<Action>
    {
        public:
            /// @brief Core pipeline type.
            using PipeType = Pipeline<Action>;

            /// @brief Construct the pipeline.
            /// @param _input Input data queue of the pipeline.
            /// @param _output Output data queue of the pipeline.
            /// @param _raw Raw message queue send to the pip::OutNetwork pipeline.
            Action(NetToAction &_input, MarketEntry &_output, RawOutput &_raw);
            /// @brief Stop and then destroy the pipeline.
            ~Action();

            /// @brief Run the pipeline
            /// @return Return true if the pipeline as correctly started else false.
            [[nodiscard]] bool start();

            /// @brief Core function of the pipeline determining it's behavior
            void loop();

        protected:
            bool treatLogon(ActionIn &_input);
            bool treatLogout(ActionIn &_input);
            bool treatNewOrderSingle(ActionIn &_input);
            bool treatOrderCancelRequest(ActionIn &_input);
            bool treatOrderCancelReplaceRequest(ActionIn &_input);
            bool treatUnknown(ActionIn &_input);
            bool treatHeartbeat(ActionIn &_input);

        private:
            NetToAction &m_input;       ///< Intput data queue.
            MarketEntry &m_output;      ///< Map of every market ouput data queue.
            RawOutput &m_raw;           ///< Raw message queue.
    };
}