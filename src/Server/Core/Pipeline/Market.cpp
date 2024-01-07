#include <future>

#include "Common/Core/Logger.hpp"
#include "Server/Core/Pipeline/Market.hpp"
#include "Server/Core/meta.hpp"
#include "Common/Message/ExecutionReport.hpp"

namespace pip
{
    Market::Market(OrderBook &_ob, SerialToMarket &_input, MarketToNet &_output)
        : m_input(_input), m_output(_output), m_ob(_ob)
    {
    }

    Market::~Market()
    {
        (void)stop();
    }

    bool Market::start()
    {
        if (!m_running)
            tstart(this);
        Logger::Log("[Market] Running: ", m_running);
        return m_running;
    }

    void Market::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "Market");
        MarketIn input;

        while (m_running) {
            // use a mutex wait?
            if (!m_input.empty()) {
                input = m_input.pop_front();
                process(input);
                m_tp.enqueue([this, _input = std::move(input)] () {
                    send(_input);
                });
            }
        }
    }

    void Market::process(MarketIn &_data)
    {
        Logger::Log("[Market] Processing action: "); // todo log
        data::UDPPackage udp;
        fix::ExecutionReport report;
        Quantity quantity = _data.OrderData.order.quantity;

        report.set40_OrdType("1");
        switch (_data.OrderData.action) {
            case OrderBook::Data::Action::Add:
                report.set44_price(std::to_string(_data.OrderData.price));
                report.set37_OrderID(std::to_string(_data.OrderData.orderId));
                report.set54_Side((_data.OrderData.type == OrderType::Ask) ? "3" : "4");
                if (m_ob.add(_data.OrderData.type, _data.OrderData.price, _data.OrderData.order)) {
                    udp.quantity = _data.OrderData.order.quantity;
                    udp.price = _data.OrderData.price;
                    UDP_FLAG_SET_ADD(udp.flag);
                    (_data.OrderData.type == OrderType::Ask) ? UDP_FLAG_SET_ASK(udp.flag) : UDP_FLAG_SET_BID(udp.flag);
                    UDP_FLAG_SET_STATUS(_flag, OrderStatus::New);
                    // send over udp
                    report.set39_OrdStatus("0");
                } else {
                    report.set39_OrdStatus((quantity == _data.OrderData.order.quantity) ? "1" : "2");
                }
                break;
            case OrderBook::Data::Action::Modify:
                m_ob.modify(_data.OrderData.type, _data.OrderData.price, _data.OrderData.oprice, _data.OrderData.order);
                break;
            case OrderBook::Data::Action::Cancel:
                m_ob.cancel(_data.OrderData.type, _data.OrderData.price, _data.OrderData.userId, _data.OrderData.orderId);
                break;
            default:
                // send an internal error message
                return;
        }
    }

    void Market::send(const MarketIn _data)
    {
    }
}