#pragma once

#include "Common/Message/Fix.hpp"
#include "Common/Message/Serializer.hpp"
#include "Common/Thread/Queue.hpp"
#include "Server/Core/ClientSocket.hpp"

#define UDP_FLAG_SET_BID(_flag)     _flag &= 00
#define UDP_FLAG_SET_ASK(_flag)     _flag &= 01
#define UDP_FLAG_GET_BOOK(_flag)    _flag & 01

#define UDP_FLAG_SET_ADD(_flag)
#define UDP_FLAG_SET_MOD(_flag)
#define UDP_FLAG_SET_CAN(_flag)
#define UDP_FLAG_GET_TYPE(_flag)    (_flag >> 1) & 011

#define UDP_FLAG_SET_STATUS(_flag, _s) _s & 0111
#define UDP_FLAG_GET_STATUS(_flag) OrderStatus((_flag >> 3) & 0111)

namespace data
{
    /// @brief Data transfered from the pip::InNetwork pipeline to the pip::Action pipeline.
    struct NetToSerial
    {
        NetToSerial() = default;
        NetToSerial(const NetToSerial &_data);

        NetToSerial &operator=(const NetToSerial &_data);

        ClientSocket Client{};                  ///< Sender client information.
        fix::Serializer::AnonMessage Message{}; ///< Undefined message data.
    };

    /// @brief Data transfered from the pip::Action pipeline to the pip::Market pipeline
    struct SerialToMarket
    {
        SerialToMarket() = default;
        SerialToMarket(const SerialToMarket &_data);

        SerialToMarket &operator=(const SerialToMarket &_data);

        ClientSocket Client{};          ///< Sender client information.
        OrderBook::Data OrderData{};    ///< Action to apply to the OrderBook.
    };

    /// @brief Data transfered from the pip::Market pipeline to the pip::OutNetwork pipeline
    struct MarketToNet
    {
        MarketToNet() = default;
        MarketToNet(const MarketToNet &_data);

        MarketToNet &operator=(const MarketToNet &_data);

        ClientSocket Client{};  ///< Sender client information.
        fix::Message Message{}; ///< Final message send to the client.
    };

    struct MarketEvent
    {
        OrderId orderId;
        OrderStatus status;
        Price price;
        Quantity remaing;
        Side side;
    };

    /// @brief Data send to the UDP broadcast in pip::UDPOutNetwork pipeline.
    struct UDPPackage
    {
        uint32_t time;
        uint8_t flag;
        Quantity quantity;
        Price price;
    };
}

/// @brief Output data type of the pip::InNetwork pipeline.
using NetOut = data::NetToSerial;
/// @brief Input data type of the pip::Action pipeline.
using SerialIn = NetOut;

/// @brief Output data type of the pip::Action pipeline.
using SerialOut = data::SerialToMarket;
/// @brief Input data type of the pip::Market pipeline.
using MarketIn = SerialOut;

/// @brief Output data type of the pip::Market pipeline.
using MarketOut = data::MarketToNet;
/// @brief Input data type of the pip::OutNetwork pipeline.
using NetIn = MarketOut;

/// @brief Queue type use to transfer data from pip::InNetwork to pip::Action pipeline.
using NetToSerial = ts::Queue<NetOut>;
/// @brief Queue type use to transfer data from pip::Action to pip::Market pipeline.
using SerialToMarket = ts::Queue<SerialOut>;
/// @brief Queue type use to transfer data from pip::Market to pip::OutNetwork pipeline.
using MarketToNet = ts::Queue<MarketOut>;
/// @brief Queue type use to transfer direct message from any pipeline to the pip::OutNetwork pipeline.
using RawOutput = ts::Queue<NetIn>;

/// @brief Queue type use to transfer data to be formated and send by the pip::UDPOutNetwork pipeline.
using UdpInput = ts::Queue<data::UDPPackage>;

using OBOutput = ts::Queue<data::MarketEvent>;