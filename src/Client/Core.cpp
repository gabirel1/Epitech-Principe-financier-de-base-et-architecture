#include "Client/Core.hpp"
#include "Client/Data/OrderBook.hpp"
#include "Client/Data/UserInput.hpp"

Core::Core(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp)
    : m_udp(_ip, _udp, m_udp_in, m_udp_out), m_input(m_entry) //, m_tcp(_ip, _tcp, m_tcp_in, m_tcp_out)
{
    std::shared_ptr<proc::OrderBook> ob = std::make_shared<proc::OrderBook>();

    m_proc_tcp.push_back(ob);
    m_proc_udp.push_back(ob);
}

Core::~Core()
{
    stop();
}

void Core::start()
{
    m_running = true;

    (void)m_udp.start();
    // m_tcp.start();
    m_input.start();
    bool found = false;

    while (m_running) {
        if (!m_udp_out.empty()) {
            found = false;
            for (auto &_proc : m_proc_udp) {
                std::optional<data::UDPPackage> res = _proc->process(m_udp_out.pop_front());

                if (res) {
                    found = true;
                    m_udp_in.push(res.value());
                    break;
                }
            }
            if (!found) {
                std::cout << "[Error] unknow message received (UDP)" << std::endl;
                m_udp_out.pop();
            }
        }
        if (!m_tcp_out.empty()) {
            found = false;
            for (auto &_proc : m_proc_tcp) {
                fix::Serializer::AnonMessage val = m_tcp_out.pop_front();
                std::optional<fix::Message> res = _proc->process(val);

                if (res) {
                    found = true;
                    m_tcp_in.push(res.value());
                    break;
                }
            }
            if (!found) {
                std::cout << "[Error] unknow message received (TCP)" << std::endl;
                m_tcp_out.pop();
            }
        }
        if (!m_entry.empty()) {
            found = false;
            for (auto &_proc : m_proc_entry) {
                std::string log = m_entry.pop_front();
                std::optional<fix::Message> res = _proc->process(log);

                if (res) {
                    found = true;
                    m_entry.push(res.value());
                    break;
                }
            }
            if (!found)
                std::cout << "[Error] unknow command: " << m_entry.pop_front() << std::endl;
        }
    }
}

void Core::stop()
{
    m_running = false;

    (void)m_udp.stop();
    // m_tcp.stop();
    (void)m_input.stop();
}