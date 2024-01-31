#include "Server/Core/Pipeline/DataRefresh.hpp"

namespace pip
{
    DataRefresh::DataRefresh(std::map<std::string, MarketContainer> &_markets, InMarketData &_input, InOutNetwork &_output)
        : m_markets(_markets), m_input(_input), m_output(_output)
    {
    }

    DataRefresh::~DataRefresh()
    {
        (void)stop();
    }

    bool DataRefresh::start()
    {
        if (!m_running)
            tstart(this);
        Logger::Log("[DataRefresh] (DataRefresh) Running: ", m_running);
        return m_running;
    }

    void DataRefresh::loop()
    {
        Logger::SetThreadName(THIS_THREAD_ID, "DataRefresh");
        MarketDataInput input;

        while (m_running) {
            if (!m_input.empty()) {
                input = m_input.pop_front();
                process(input);
            }
        }
    }

    void DataRefresh::process(MarketDataInput &_input)
    {
        if (_input.SubType == 0) {
            OrderBook::Subscription sub;
            fix::MarketDataSnapshotFullRefresh message;

            message.set262_mDReqID(_input.Id);
            for (const auto &_sym : _input.Symbols) {
                for (OrderType _type : _input.Types) {
                    sub.depth = _input.Depth;
                    sub.type = _type;
                    message += m_markets.at(_sym).refresh(sub);
                }
                m_markets.at(_sym).cache_flush();
                message.set55_symbol(_sym);
                m_output.append(std::move(_input.Client), std::move(message));
            }
        } else if (_input.SubType == 1) {
            OrderBook::Subscription sub;

            sub.depth = _input.Depth;
            sub.Id = _input.Id;
            for (const auto &_sym : _input.Symbols) {
                std::vector<OrderBook::Subscription> &subs = _input.Client.subscribe(_sym);

                for (OrderType _type : _input.Types) {
                    sub.type = _type;
                    subs.push_back(sub);
                }
                fix::MarketDataSnapshotFullRefresh message;
                message.set55_symbol(_sym);
                message.set262_mDReqID(_input.Id);
                message += m_markets.at(_sym).refresh(sub);
                m_output.append(std::move(_input.Client), std::move(message));
            }
        } else {
            for (const auto &_sym : _input.Symbols) {
                std::vector<OrderBook::Subscription> &subs = _input.Client.subscribe(_sym);

                for (OrderType _type : _input.Types)
                    for (std::vector<OrderBook::Subscription>::iterator it = subs.begin(); it != subs.end();)
                        if (_input.Id == it->Id && _type == it->type)
                            subs.erase(it);
            }
            // what to send???
        }
    }
}