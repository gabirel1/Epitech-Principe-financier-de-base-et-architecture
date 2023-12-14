#pragma once

#include "Core/ClientSocket.hpp"
#include "Message/Fix.hpp"
#include "Message/Serializer.hpp"
#include "Thread/Queue.hpp"

namespace data
{
    class NetToSerial
    {
        public:
            NetToSerial() = default;
            NetToSerial(NetToSerial &&_data) noexcept;
            ~NetToSerial() = default;

            ClientSocket Client{};
            fix::Serializer::AnonMessage Message{};
    };

    struct SerialToMarket
    {
        ClientSocket Client{};
        OrderBook::Data OrderData{};
    };

    class MarketToNet
    {
        public:
            MarketToNet() = default;
            MarketToNet(const MarketToNet &&_data) noexcept;
            ~MarketToNet() = default;
            ClientSocket Client{};
            fix::Message Message{};
    };
}

using NetOut = data::NetToSerial;
using SerialIn = NetOut;

using SerialOut = data::SerialToMarket;
using MarketIn = SerialOut;

using MarketOut = data::MarketToNet;
using NetIn = MarketOut;

using NetToSerial = ts::Queue<NetOut>;
using SerialToMarket = ts::Queue<SerialOut>;
using MarketToNet = ts::Queue<MarketOut>;
using RawOutput = ts::Queue<NetIn>;