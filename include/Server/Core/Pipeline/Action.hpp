#pragma once

#include "Server/Core/Pipeline/Core.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

namespace pip
{
    class Action : public Pipeline<Action>
    {
        public:

            using PipeType = Pipeline<Action>;

            Action(NetToSerial &_input, SerialToMarket &_output, RawOutput &_raw);
            ~Action();

            [[nodiscard]] bool start();
            void status(float _to = 1.f);

            void loop();

        protected:
            bool treatLogon(SerialIn &_input);
            bool treatLogout(SerialIn &_input);
            bool treatNewOrderSingle(SerialIn &_input);

        private:
            NetToSerial &m_input;
            SerialToMarket &m_output;
            RawOutput &m_raw;
    };
}