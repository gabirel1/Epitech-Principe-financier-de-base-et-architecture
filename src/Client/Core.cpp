#include "Client/Core.hpp"
#include "Client/Processor/OrderBook.hpp"
#include "Client/Processor/User.hpp"

Core::Core(const net::Ip &_ip, uint32_t _tcp, uint32_t _udp)
    : m_tcp(_ip, _tcp), m_udp(_ip, _udp)
{
    std::shared_ptr<proc::OrderBook> ob = std::make_shared<proc::OrderBook>();
    std::shared_ptr<proc::User> user = std::make_shared<proc::User>();

    m_proc_tcp.push_back(ob);
    m_proc_tcp.push_back(user);

    m_proc_udp.push_back(ob);

    m_proc_entry.push_back(user);
}

Core::~Core()
{
    stop();
}

void Core::start()
{
    m_running = true;

    (void)m_udp.start();
    m_tcp.start();
    m_input.start();

    while (m_running) {
        if (!m_udp.empty(io::Side::Recv)) {
            const data::UDPPackage package = m_udp.pop_front_recv();

            for (auto &_proc : m_proc_udp) {
                std::optional<data::UDPPackage> res = _proc->process(package, m_context);

                if (res) {
                    m_udp.send_to_send(std::move(res.value()));
                    break;
                }
            }
        }
        if (!m_tcp.empty(io::Side::Recv)) {
            fix::Serializer::AnonMessage val = m_tcp.pop_front_recv();

            for (auto &_proc : m_proc_tcp) {
                std::optional<fix::Message> res = _proc->process(val, m_context);

                if (res) {
                    m_tcp.send_to_send(std::move(res.value()));
                    break;
                }
            }
        }
        if (!m_input.empty(io::Side::Recv)) {
            const std::string entry = m_input.pop_front_recv();

            for (auto &_proc : m_proc_entry) {
                std::optional<fix::Message> res = _proc->process(entry, m_context);

                if (res) {
                    m_input.send_to_send(std::move(res.value()));
                    break;
                }
            }
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