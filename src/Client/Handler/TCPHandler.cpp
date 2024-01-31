#include "Client/Handler/TCPHandler.hpp"

namespace io
{
    TCPHandler::TCPHandler(const net::Ip &_ip, uint32_t _port)
    {
        m_socket = std::make_unique<net::tcp::Socket>();
        m_socket->connect(_ip, _port);
    }

    void TCPHandler::loop()
    {
        int error = 0;
        std::vector<Client> clients{1};

        while (*this) {
            clients = m_selector.pull();

            for (Client &_client : clients) {
                fix::Serializer::AnonMessage msg;
                std::string data(_client->receive(MAX_RECV_SIZE, error));

                if (error == 0)
                    continue;
                if (fix::Serializer::run(data, msg) != fix::Serializer::Error::None)
                    continue;
                send_to_recv(std::move(msg));
            }
            if (!empty(Side::Send))
                m_socket->send(pop_front_send().to_string());
        }
    }
}