#pragma once

#include "Common/Network/Selector.hpp"
#include "Server/Core/Pipeline/Core.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

#ifndef NET_RECV_SIZE
    #define NET_RECV_SIZE 4096
#endif

namespace pip
{
    /// @brief Pipeline managing the Network input.
    /// @tparam T is the socket type managed.
    template<IsSocket T>
    class InNetwork : public Pipeline<InNetwork<T>>
    {
        public:
            /// @brief Socket format used
            using Client = net::Acceptor<T>::Client;

            /// @brief Core pipeline type
            using PipeType = Pipeline<InNetwork<T>>;

            /// @brief Setup the network input and start listening on _port
            /// @param _clients List of clients connected in to this network
            /// @param _output Output data queue of the pipeline.
            /// @param _port Port on which the network will listen to, for connection and receive message.
            InNetwork(std::vector<ClientSocket> &_clients, NetToSerial &_output, RawOutput &_error, uint32_t _port);
            /// @brief Disconnect from listening, stop the pipeline and the destroy it. 
            ~InNetwork();

            /// @brief Run the pipeline
            /// @return Return true if the pipeline as correctly started else false.
            [[nodiscard]] bool start();

            /// @brief Core function of the pipeline determining it's behavior
            void loop();

        protected:
            /// @brief Function to process an action on a Socket.
            /// @param _client socket with a pending action required.
            void process(ClientSocket &_client);

        private:
            std::vector<ClientSocket> &m_clients;   ///< Client list
            NetToSerial &m_output;                  ///< Ouput data queue
            RawOutput &m_error;                     ///< Output message queue directly to the pip::OutNetwork

            net::Acceptor<T> m_acceptor;            ///< Acceptor listening the the port passed as parameter.
            net::Selector<T> m_selector;            ///< Selector managing the client list action.
    };
}

#include "Server/Core/Pipeline/InNetwork.inl"
