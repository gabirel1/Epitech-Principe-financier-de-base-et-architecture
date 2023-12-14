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
    class InNetwork : public Pipeline<InNetwork<T>>
    {
        public:
            using Client = net::Acceptor<T>::Client;

            using PipeType = Pipeline<InNetwork<T>>;

            InNetwork(std::vector<ClientSocket> &_clients, NetToSerial &_output, uint32_t _port);
            ~InNetwork();

            [[nodiscard]] bool start();
            void status(float _to = 1.f);

            void loop();

        protected:
            void process(Client _client);

        private:
            std::vector<ClientSocket> &m_clients; // implement a thread safe vector
            NetToSerial &m_output;

            net::Acceptor<T> m_acceptor;
            net::Selector<T> m_selector;
    };
}

#include "Core/Pipeline/InNetwork.inl"