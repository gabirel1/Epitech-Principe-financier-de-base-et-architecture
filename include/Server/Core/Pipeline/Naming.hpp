#pragma once

#include "Common/Message/Fix.hpp"
#include "Common/Message/Serializer.hpp"
#include "Common/Thread/Queue.hpp"
#include "Server/Core/ClientSocket.hpp"

namespace data
{
    struct NetToSerial
    {
        NetToSerial() = default;
        NetToSerial(const NetToSerial &_data);

        NetToSerial &operator=(const NetToSerial &_data);

        ClientSocket Client{};
        fix::Serializer::AnonMessage Message{};
    };

    struct SerialToMarket
    {
        SerialToMarket() = default;
        SerialToMarket(const SerialToMarket &_data);

        SerialToMarket &operator=(const SerialToMarket &_data);

        ClientSocket Client{};
        OrderBook::Data OrderData{};
    };

    struct MarketToNet
    {
        MarketToNet() = default;
        MarketToNet(const MarketToNet &_data);

        MarketToNet &operator=(const MarketToNet &_data);

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