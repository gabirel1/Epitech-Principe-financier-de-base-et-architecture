#pragma once

#include "Core/Pipeline/Core.hpp"
#include "Core/Pipeline/Naming.hpp"
#include "Network/Selector.hpp"

#ifndef NET_RECV_SIZE
    #define NET_RECV_SIZE 4096
#endif

namespace pip
{
    template<IsSocket T>
    class OutNetwork : public Pipeline<OutNetwork<T>>
    {
        public:
            using Client = net::Acceptor<T>::Client;

            using PipeType = Pipeline<OutNetwork<T>>;

            OutNetwork(MarketToNet &_input);
            ~OutNetwork();

            [[nodiscard]] bool start();
            void status(float _to = 1.f);

            void loop();

        protected:
            void process(Client _client);

        private:
            MarketToNet &m_input;
    };
}

#include "Core/Pipeline/OutNetwork.inl"