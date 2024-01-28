#include "Client/Data/UDPHandler.hpp"

UDPHandler::UDPHandler(uint32_t _port, const Ip &_ip, UDPInput &_input, UDPOutput &_output)
    : m_ip(_ip), m_input(_input), m_output(_output)
{
    m_socket = std::make_shared<net::udp::Socket>();
    m_socket->connect(_ip, _port);
    m_socket->blocking(false);
    m_selector->blockgin(false);
}

UDPHandler::~UDPHandler()
{
    stop();
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

void UDPHandler::loop()
{
    m_selector.client(m_socket);
    std::vector<Client> clients;
    int error = 0;

    while (m_running) {
        clients = m_selector.pull();
        if (!clients.empty()) {
            const data::UDPPackage *package = reinterpret_cast<const data::UDPPackage *>(m_socket->receiveUDP(sizeof(data::UDPPackage), error));

            m_output.push(std::move(*package));
        }
        if (!m_input.empty()) {
            data::UDPPackage package = m_input.front();
            m_socket->send(reinterpret_cast<const uint8_t *>(&pacakge), sizeof(data::UDPPackage));
        }
    }
}