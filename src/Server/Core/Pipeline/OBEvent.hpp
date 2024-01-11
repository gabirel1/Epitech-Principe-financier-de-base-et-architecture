#include "Server/Core/Pipeline/OBEvent.hpp"

namespace pip
{
    OBEvent::OBEvent(OrderBook::EventQueue &_input, UdpInput &_udp, RawOutput &_tcp)
        : m_input(_input), m_udp(_udp), m_tcp(_tcp)
    {
    }

    OBEvent::~OBEvent()
    {
        (void)stop();
    }

    bool OBEvent::start()
    {
        if (!m_running)
            tstart(this);
        Logger::Log("[OBEvent] Running: ", m_running);
        return m_running;
    }

    void OBEvent::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "Order Book Event");

        OrderBook::Event input;

        while (m_running) {
            if (!m_input.empty()) {
                input = m_input.pop_front();

                m_tp.enqueue([this, input = std::move(input)] () {
                    createUdp(input);
                    createTcp(input);
                });
            }
        }
    }

    bool createTcp(OrderBook::Event &input)
    {
        return true;
    }

    bool createUdp(OrderBook::Event &input)
    {
        return true;
    }
}