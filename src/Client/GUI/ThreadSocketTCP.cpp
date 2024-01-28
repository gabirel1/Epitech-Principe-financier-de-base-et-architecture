#include "Client/GUI/ThreadSocketTCP.hpp"

ThreadSocketTCP::ThreadSocketTCP(const net::Ip &_ip, uint32_t _port)
    : m_ip(_ip), m_port(_port), m_tcp(nullptr)
{
    m_running = true;
    m_tcp = std::make_shared<net::tcp::Socket>();
    m_tcp->connect(m_ip, m_port);

    m_select.client(m_tcp);
}

ThreadSocketTCP::~ThreadSocketTCP()
{
}

void ThreadSocketTCP::run()
{
    int error = 0;
    std::vector<Client> clients;

    while (m_running == true) {
        clients = m_select.pull();
        if (m_clients.size())
            Logger::Log("[InNetwork] Received event from: ", clients.size(), " clients");
        for (auto &_client : clients) {
            // check for dc or any error
            std::string res = _client->receive(1024, error);
            fix::Serializer::AnonMessage msg;

            if (fix::Serializer::run(res, msg) != fix::Serializer::Error::None) {
                // error dont handle
            }
            m_output.emplace(std::move(msg));
        }

        if (!m_input.empty()) {
            std::string msg = m_input.pop_front().to_string();

            if (m_tcp->send(msg))
                Logger::Log("Sending error"); // todo log
        }
    }
}
