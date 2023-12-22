#include <future>

#include "Common/Core/Logger.hpp"
#include "Server/Core/Pipeline/Market.hpp"
#include "Server/Core/meta.hpp"

namespace pip
{
    Market::Market(OrderBook &_ob, SerialToMarket &_input, MarketToNet &_output)
        : m_input(_input), m_output(_output), m_ob(_ob)
    {
        
    }

    Market::~Market()
    {
        stop();
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

        switch (_data.OrderData.action) {
            case OrderBook::Data::Action::Add:
                m_ob.add(_data.OrderData.type, _data.OrderData.price, _data.OrderData.order);
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