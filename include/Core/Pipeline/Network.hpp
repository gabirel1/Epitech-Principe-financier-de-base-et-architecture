#pragma once

#include "Core/Pipeline/Core.hpp"
#include "Core/Pipeline/Naming.hpp"
#include "Network/Acceptor.hpp"
#include "Network/Selector.hpp"

#ifndef NET_RECV_SIZE
    #define NET_RECV_SIZE 4096
#endif

namespace pip
{
    template<IsSocket T>
    class Network : public Pipeline<Network<T>>
    {
        public:
            using Client = Acceptor<T>::Client;

            Network(std::vector<Client> &_clients, NetToSerial &_output, int _port);
            ~Network();

            [[nodiscard]] bool start();
            void status(float _to = 1.f);

            void loop();

        protected:
            void process(Client _client);

        private:
            std::vector<Client> &m_client; // implement a thread safe vector
            NetToSerial &m_output;

            net::Acceptor<T> m_acceptor;
            net::Selector<T> m_selector;
    }
}