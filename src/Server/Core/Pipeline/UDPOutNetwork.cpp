#include <future>

#include "Common/Core/Logger.hpp"
#include "Server/Core/Pipeline/UDPOutNetwork.hpp"

namespace pip
{
    UDPOutNetwork::UDPOutNetwork(UdpInput &_input, uint32_t _port)
        : m_input(_input)
    {
        if (!m_socket.broadcastOn(_port))
            Logger::Log("[UDPOutNetwork] Failed to setup broadcast, crashing after first action");
    }

    UDPOutNetwork::~UDPOutNetwork()
    {
        (void)stop();
    }

    bool UDPOutNetwork::start()
    {
        if (!m_running)
            tstart(this);
        Logger::Log("[UDPOutNetwork] Running: ", m_running);
        return m_running;
    }

    void UDPOutNetwork::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "UDP Network Output");

        while (m_running) {
            auto now = std::chrono::steady_clock::now();

            for (size_t it = 0; it < UDP_MAX_MSG && !m_input.empty(); it++) {
                m_message.emplace_back(now, reinterpret_cast<const uint8_t*>(&m_input.front()));
                m_input.pop();
            }
            clean();
            for (const auto &[_, _val] : m_message)
                (void)m_socket.broadcast(_val, sizeof(data::UDPPackage));
            sleep(UDP_TICK);
        }
    }

    void UDPOutNetwork::clean()
    {
        constexpr const auto clean_after =  std::chrono::seconds(UDP_CLEAR);
        auto now = std::chrono::steady_clock::now();
        size_t cleaned = 0;

        for (auto it = m_message.begin(); it != m_message.end(); ) {
            if (std::chrono::duration_cast<std::chrono::seconds>(now - it->first).count() > 2) {
                it = m_message.erase(it);
                cleaned++;
            } else {
                it++;
            }
        }
        if (cleaned)
            Logger::Log("[UDPOutNetwork] Cleaned ", cleaned, " messages from the UDP queue");
    }
}