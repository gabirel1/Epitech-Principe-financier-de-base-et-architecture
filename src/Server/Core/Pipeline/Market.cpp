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
            if (!m_input.empty()) {
                input = m_input.pop_front();
                process(input);
            }
        }
    }

    void Market::process(MarketIn &_data)
    {
        Logger::Log("[Market] Processing action: "); // todo log
        Quantity quantity = _data.OrderData.order.quantity;
        bool result = false;

        switch (_data.OrderData.action) {
            case OrderBook::Data::Action::Add:
                std::cout << "process add" << std::endl; // todo remove
                result = m_ob.add(_data.OrderData.type, _data.OrderData.price, _data.OrderData.order);
                std::cout << "process add result: " << result << std::endl; // todo remove
                m_tp.enqueue([this, _data = std::move(_data), quantity, result] () {
                    buildAdd(_data, quantity, result);
                });
                break;
            case OrderBook::Data::Action::Modify:
                m_ob.modify(_data.OrderData.type, _data.OrderData.price, _data.OrderData.order);
                break;
            case OrderBook::Data::Action::Cancel:
                m_ob.cancel(_data.OrderData.type, _data.OrderData.order);
                break;
            default:
                // send an internal error message
                return;
        }
    }

    void Market::buildAdd(const MarketIn &_data, Quantity _quantity, bool _result)
    {
        std::cout << "build add " << std::endl; // todo remove
        data::UDPPackage udp;
        fix::ExecutionReport report;

        report.set20_execTransType("1");
        report.set38_orderQty(std::to_string(_quantity));
        report.set40_ordType("2");
        report.set44_price(std::to_string(_data.OrderData.price));
        report.set37_orderID(std::to_string(_data.OrderData.order.orderId));
        report.set54_side((_data.OrderData.type == OrderType::Ask) ? "3" : "4");

        std::cout << "result" << _result << std::endl;
        if (_result) { // added
            udp.quantity = _data.OrderData.order.quantity;
            udp.price = _data.OrderData.price;
            UDP_FLAG_SET_ADD(udp.flag);
            (_data.OrderData.type == OrderType::Ask) ? UDP_FLAG_SET_ASK(udp.flag) : UDP_FLAG_SET_BID(udp.flag);
            UDP_FLAG_SET_STATUS(udp.flag, OrderStatus::New);
            // send over udp

            report.set14_cumQty(std::to_string(_quantity - _data.OrderData.order.quantity));
            report.set151_leavesQty(std::to_string(_data.OrderData.order.quantity));
            if (_data.OrderData.order.quantity == 0)
                report.set39_ordStatus("2");
            else
                report.set39_ordStatus("0");
        } else {
            report.set39_ordStatus("0");
            report.set14_cumQty(std::to_string(_quantity));
            report.set151_leavesQty("0");
        }
        report.set17_execID();
        m_output.append(data::MarketToNet{ _data.Client, report });
    }
}