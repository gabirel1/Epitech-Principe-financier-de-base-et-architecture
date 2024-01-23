#pragma once

#include "Server/Core/OrderBook.hpp"
#include "Server/Core/Pipeline/Market.hpp"
#include "Server/Core/Pipeline/Naming.hpp"
#include "Server/Core/Pipeline/OBEvent.hpp"

class MarketContainer
{
    public:
        MarketContainer(const std::string &_name, UdpInput &_udp, RawOutput &_tcp);
        ~MarketContainer();

        using ThreadStatus = std::tuple<std::future_status, std::future_status>;

        [[nodiscard]] bool start();

        [[nodiscard]] ThreadStatus stop();

        bool status(float _to = 1.f);

        [[nodiscard]] const std::string &getName() const;
        [[nodiscard]] ActionToMarket &getInput();

    private:
        const std::string m_name;

        OrderBook::EventQueue m_q_event;
        ActionToMarket m_q_action;

        OrderBook m_ob;

        pip::Market m_market;
        pip::OBEvent m_obevent;
};