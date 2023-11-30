#include "Core/Pipeline/Market.hpp"

namespace pip
{
    Market::Market(OrderBook &_ob, SerialToMarket &_input, MarketToNet &_output)
        : m_ob(_ob), m_input(_input), m_output(_output)
    {
    }

    Market::~Market()
    {
        stop();
    }

    bool Market::start()
    {
        if (!m_running) {
            m_running = true;
            tstart(this);
        }
        return m_running;
    }

    void Market::status(float _to)
    {
        tstatus(static_cast<ms>(_to * 1000));
    }

    void Market::loop()
    {
        MarketIn input;

        while (m_running) {
            // use a mutex wait?
            if (!m_input.empty()) {
                input = m_input.pop_front();
                m_tp.enqueue([this, input]() {
                    process(input);
                });
            }
        }
    }

    void Market::process(MarketIn _data)
    {
        switch (_data.action) {
            case OrderBook::Data::Action::Add:
                m_ob.add(_data.type, _data.price, _data.order);
                break;
            case OrderBook::Data::Action::Modify:
                m_ob.modify(_data.type, _data.price, _data.oprice, _data.order);
                break;
            case OrderBook::Data::Action::Cancel:
                m_ob.cancel(_data.type, _data.price, _data.userId, _data.orderId);
                break;
            default:
                // send an internal error message
                return;
        }
        m_tp.enqueue([this, _data] () {
            send(_data);
        });
    }

    void Market::send(MarketIn _data)
    {
        MarketOut out;

        // build base communication packet for network output
        m_output.append(std::move(out));
    }
}