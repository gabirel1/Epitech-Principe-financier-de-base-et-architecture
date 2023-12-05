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
    template<class T>
    class Network;

    template<IsSocket T>
    class Network<typename net::Acceptor<T>::Client> : public Pipeline<Network<T>>
    {
        public:
            using Client = net::Acceptor<T>::Client;

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
    };
}

#include "Core/Pipeline/Network.hpp"