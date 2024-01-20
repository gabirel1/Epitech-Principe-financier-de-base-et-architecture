#pragma once

#include "Common/Message/Fix.hpp"
#include "Common/Message/Serializer.hpp"
#include "Common/Thread/Queue.hpp"
#include "Server/Core/OrderBook.hpp"
#include "Server/Core/ClientSocket.hpp"

#define UDP_FLAG_SET_BID(_flag)         _flag |= 01
#define UDP_FLAG_SET_ASK(_flag)         _flag &= ~01
#define UDP_FLAG_GET_BOOK(_flag)        OrderType(_flag & 01)

#define UDP_FLAG_SET_STATUS(_flag, _s)  _flag = (_flag & ~01110) | ((_s & 0111) << 1)
#define UDP_FLAG_GET_STATUS(_flag)      OrderStatus((_flag >> 1) & 0111)

#define UDP_FLAG_SET_SOLD(_flag, _s)    _flag = (_flag & ~010000) | ((_s & 01) << 4)
#define UDP_FLAG_GET_IS_SOLD(_flag)          bool((_flag >> 4) & 01)

namespace data
{
    /// @brief Data transfered from the pip::InNetwork pipeline to the pip::Action pipeline.
    struct NetToSerial
    {
        NetToSerial() = default;
        NetToSerial(const NetToSerial &_data);
        NetToSerial(const ClientSocket &_client, const fix::Serializer::AnonMessage &_message);

        NetToSerial &operator=(const NetToSerial &_data);

        ClientSocket Client{};                      ///< Sender client information.
        fix::Serializer::AnonMessage Message{};     ///< Undefined message data.
    };

    /// @brief Data transfered from the pip::Action pipeline to the pip::Market pipeline
    struct SerialToMarket
    {
        SerialToMarket() = default;
        SerialToMarket(const SerialToMarket &_data);

        SerialToMarket &operator=(const SerialToMarket &_data);

        ClientSocket Client{};                              ///< Sender client information.
        OrderBook::Data OrderData{};                        ///< Action to apply to the OrderBook.
    };

    /// @brief Data transfered from the pip::Market pipeline to the pip::OutNetwork pipeline
    struct MarketToNet
    {
        MarketToNet() = default;
        MarketToNet(const MarketToNet &_data);
        MarketToNet(const ClientSocket &_client, const fix::Message &_msg);

        MarketToNet &operator=(const MarketToNet &_data);

        ClientSocket Client{};                      ///< Sender client information.
        fix::Message Message{};                     ///< Final message send to the client.
    };

    /// @brief Data send to the UDP broadcast in pip::UDPOutNetwork pipeline.
    struct UDPPackage
    {
        uint32_t time;
        uint64_t id;
        uint8_t flag;
        Quantity quantity;
        Price price;
    };

    std::ostream &operator<<(std::ostream &_os, const UDPPackage &_package);
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
using ErrorMsg = NetIn;

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
