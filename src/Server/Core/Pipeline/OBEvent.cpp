#include "Server/Core/Pipeline/OBEvent.hpp"
#include "Common/Message/ExecutionReport.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

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

                m_tp.enqueue([this, input] () {
                    createUdp(input);
                    createTcp(input);
                });
            }
        }
    }

    bool OBEvent::createTcp(OrderBook::Event _input)
    {
        fix::ExecutionReport report;

        report.set14_cumQty(std::to_string(_input.orgQty - _input.quantity));
        report.set17_execID();
        report.set20_execTransType("1");
        report.set38_orderQty(std::to_string(_input.orgQty));
        report.set37_orderID(std::to_string(_input.orderId));
        report.set39_ordStatus(std::to_string(static_cast<uint8_t>(_input.status)));
        report.set40_ordType("2");
        report.set44_price(std::to_string(_input.price));
        report.set54_side((_input.side == OrderType::Ask) ? "3" : "4");
        report.header.set56_TargetCompId(std::to_string(_input.userId));
        report.set151_leavesQty(std::to_string(_input.quantity));
        m_tcp.append(NetIn{ {}, report });
        return true;
    }

    bool OBEvent::createUdp(OrderBook::Event _input)
    {
        data::UDPPackage package;

        if (_input.status == OrderStatus::Pending || _input.quantity == 0)
            return false;
        package.time = 0;   // todo
        package.id = m_id++;
        package.flag = 0;
        UDP_FLAG_SET_SOLD(package.flag, _input.sold);
        UDP_FLAG_SET_STATUS(package.flag, _input.status);
        (_input.side == OrderType::Ask) ? UDP_FLAG_SET_ASK(package.flag) : UDP_FLAG_SET_BID(package.flag);
        package.quantity = _input.quantity;
        package.price = _input.price;
        m_udp.append(package);
        return true;
    }
}