#pragma once

#include "Common/Message/Fix.hpp"
#include "Common/Message/Serializer.hpp"
#include "Common/Thread/Queue.hpp"
#include "Server/Core/ClientSocket.hpp"

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
        std::chrono::_V2::system_clock::time_point Time{};  ///< Time of the action.
    };

    /// @brief Data transfered from the pip::Action pipeline to the pip::Market pipeline
    struct SerialToMarket
    {
        SerialToMarket() = default;
        SerialToMarket(const SerialToMarket &_data);

        SerialToMarket &operator=(const SerialToMarket &_data);

        ClientSocket Client{};                              ///< Sender client information.
        OrderBook::Data OrderData{};                        ///< Action to apply to the OrderBook.
        std::chrono::_V2::system_clock::time_point Time{};  ///< Time of the action.
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
        std::chrono::_V2::system_clock::time_point Time{};  ///< Time of the action.
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
using ErrorMsg = NetIn;

/// @brief Queue type use to transfer data from pip::InNetwork to pip::Action pipeline.
using NetToSerial = ts::Queue<NetOut>;
/// @brief Queue type use to transfer data from pip::Action to pip::Market pipeline.
using SerialToMarket = ts::Queue<SerialOut>;
/// @brief Queue type use to transfer data from pip::Market to pip::OutNetwork pipeline.
using MarketToNet = ts::Queue<MarketOut>;
/// @brief Queue type use to transfer direct message from any pipeline to the pip::OutNetwork pipeline.
using RawOutput = ts::Queue<NetIn>;