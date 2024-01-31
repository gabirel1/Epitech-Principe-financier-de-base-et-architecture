#include "Client/Data/UDPHandler.hpp"

UDPHandler::UDPHandler(const net::Ip &_ip, uint32_t _port, UDPInput &_input, UDPOutput &_output)
    : m_ip(_ip), m_input(_input), m_output(_output)
{
    m_socket = std::make_shared<net::udp::Socket>();
    m_socket->connect(_ip, _port);
    m_socket->blocking(false);
    m_selector.timeout(100);
}

UDPHandler::~UDPHandler()
{
    (void)stop();
}

bool UDPHandler::start()
{
    if (!m_running) {
        m_thread = std::thread([this] () {
            loop();
        });
        m_running = true;
    }
    return m_running;
}

bool UDPHandler::stop()
{
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
        return true;
    }
    return false;
}

uint32_t UDPHandler::port() const
{
    return m_socket->getPort();
}

net::Ip UDPHandler::ip() const
{
    return m_ip;
}

void UDPHandler::loop()
{
    m_selector.client(m_socket);
    std::vector<Client> clients;
    int error = 0;

    while (m_running) {
        clients = m_selector.pull();
        if (!clients.empty()) {
            const data::UDPPackage *package = reinterpret_cast<const data::UDPPackage *>(m_socket->receiveUDP(sizeof(data::UDPPackage), error).c_str());

            m_output.push(std::move(*package));
        }
        if (!m_input.empty()) {
            data::UDPPackage package = m_input.pop_front();
            m_socket->send(reinterpret_cast<const uint8_t *>(&package), sizeof(data::UDPPackage));
        }
    }
}