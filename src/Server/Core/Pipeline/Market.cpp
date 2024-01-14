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

        switch (_data.OrderData.action) {
            case OrderBook::Data::Action::Add:
                m_tp.enqueue([this, _data] () {
                    runAdd(_data);
                });
                break;
            case OrderBook::Data::Action::Modify:
                // m_ob.modify(_data.OrderData.type, _data.OrderData.price, _data.OrderData.order);
                break;
            case OrderBook::Data::Action::Cancel:
                m_tp.enqueue([this, _data] () {
                    runCancel(_data);
                });
                break;
            default:
                // send an internal error message
                return;
        }
    }

    bool Market::runAdd(MarketIn _data)
    {
        fix::ExecutionReport report;

        if (!m_ob.add(_data.OrderData.type, _data.OrderData.price, _data.OrderData.order)) {
            report.set14_cumQty("0");
            report.set17_execID();
            report.set20_execTransType("1");
            report.set37_orderID(std::to_string(_data.OrderData.order.orderId));
            report.set38_orderQty(std::to_string(_data.OrderData.order.quantity));
            report.set39_ordStatus("8");
            report.set40_ordType("2");
            report.set44_price(std::to_string(_data.OrderData.price));
            report.set54_side((_data.OrderData.type == OrderType::Ask) ? "3" : "4");
            report.set58_text("Order Id already used");
            report.set151_leavesQty("0");
            m_output.append(data::MarketToNet{ _data.Client, report });
            return false;
        }
        return true;
    }

    bool Market::runModify(MarketIn _data)
    {
        if (!m_ob.has(_data.OrderData.type, _data.OrderData.order.orderId)) {
            return false;
        } else {
            // pending
            if (m_ob.modify(_data.OrderData.type, _data.OrderData.price, _data.OrderData.order)) {
                return false;
            }
        }
        return true;
    }

    bool Market::runCancel(MarketIn _data)
    {
        if (!m_ob.cancel(_data.OrderData.type, _data.OrderData.order.orderId)) {
            // reject
            return false;
        }
        return true;
    }
}