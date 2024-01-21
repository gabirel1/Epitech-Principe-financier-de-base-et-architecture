#include <future>

#include "Common/Core/Logger.hpp"
#include "Server/Core/Pipeline/Market.hpp"
#include "Server/Core/meta.hpp"
#include "Common/Message/ExecutionReport.hpp"
#include "Common/Message/OrderCancelReject.hpp"

namespace pip
{
    Market::Market(const std::string &_name, OrderBook &_ob, ActionToMarket &_input, MarketToNet &_output)
        : m_name(_name), m_input(_input), m_output(_output), m_ob(_ob)
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
        Logger::Log("[Market] (", m_name ,") Running: ", m_running);
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
        Logger::Log("[Market] (", m_name ,") Processing new action: "); // todo log

        switch (_data.OrderData.action) {
            case OrderBook::Data::Action::Add:
                m_tp.enqueue([this, _data] () {
                    runAdd(_data);
                });
                break;
            case OrderBook::Data::Action::Modify:
                m_tp.enqueue([this, _data] () {
                    runModify(_data);
                });
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

        Logger::Log("[Market] (New) request: "); // todo log
        if (!m_ob.add(_data.OrderData.type, _data.OrderData.price, _data.OrderData.order)) {
            Logger::Log("[Market] (New) Reject: Order ID already used: ", _data.OrderData.order.orderId);
            report.set14_cumQty("0");
            report.set17_execID();
            report.set20_execTransType("1");
            report.set37_orderID(_data.OrderData.order.orderId);
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
        Logger::Log("[Market] (New) Order executaded sucefully: ", _data.OrderData.order, ", price: ", _data.OrderData.price);
        return true;
    }

    bool Market::runModify(MarketIn _data)
    {
        fix::OrderCancelReject report;

        Logger::Log("[Market] (Modify) Request: "); // todo log
        report.set37_orderID(_data.OrderData.target);
        report.set11_clOrdID(_data.OrderData.target);
        report.set41_origClOrdID(_data.OrderData.order.orderId);
        if (!m_ob.cancel(_data.OrderData.type, _data.OrderData.target, false)) {
            report.set39_ordStatus("8");
            report.set58_text("Order ID doesn't exist");
            Logger::Log("[Market] (Modify-Cancel) Reject: Order ID already exist: ", _data.OrderData.target);
            return false;
        } else if (!m_ob.modify(_data.OrderData.type, _data.OrderData.price, _data.OrderData.order)) {
            report.set39_ordStatus("4");
            report.set58_text("Order ID already exist, target got canceled");
            Logger::Log("[Market] (Modify-Add) Reject: Order ID already exist: ", _data.OrderData.order.orderId);
            m_output.append(data::MarketToNet{ _data.Client, report });
            return false;
        }
        Logger::Log("[Market] (Modify) Order modify sucessfully: ", _data.OrderData.target, " -> ", _data.OrderData.order);
        return true;
    }

    bool Market::runCancel(MarketIn _data)
    {
        fix::OrderCancelReject report;

        Logger::Log("[Market] (Cancel) Request: ", _data.OrderData.order.orderId);
        if (!m_ob.cancel(_data.OrderData.type, _data.OrderData.order.orderId)) {
            Logger::Log("[Market] (Cancel) Reject: Order ID not found: ", _data.OrderData.order.orderId);
            report.set11_clOrdID(_data.OrderData.order.orderId);
            report.set37_orderID(_data.OrderData.order.orderId);
            report.set41_origClOrdID(_data.OrderData.order.orderId);
            report.set434_cxlRejReason("8");
            report.set58_text("Order ID doesn't exist");
            m_output.append(data::MarketToNet{ _data.Client, report });
            return false;
        }
        Logger::Log("[Market] (Cancel) Sucessfuly executed on: ", _data.OrderData.order.orderId);
        return true;
    }
}