#pragma once

#include "Common/Message/Fix.hpp"
#include "Common/Message/Serializer.hpp"
#include "Common/Thread/Queue.hpp"
#include "Server/Core/OrderBook.hpp"
#include "Server/Core/ClientSocket.hpp"
#include "Common/Network/UDPPackage.hpp"

namespace data
{
    /// @brief Data transfered from the pip::InNetwork pipeline to the pip::Action pipeline.
    struct NetToAction
    {
        NetToAction() = default;
        NetToAction(const NetToAction &&_data) noexcept;
        NetToAction(const ClientSocket &_client, const fix::Serializer::AnonMessage &&_message) noexcept;

        NetToAction &operator=(NetToAction &&_data) noexcept;

        ClientSocket Client{};                      ///< Sender client information.
        fix::Serializer::AnonMessage Message{};     ///< Undefined message data.
    };

    /// @brief Data transfered from the pip::Action pipeline to the pip::Market pipeline
    struct ActionToMarket
    {
        ActionToMarket() = default;
        ActionToMarket(const ActionToMarket &&_data) noexcept;
        ActionToMarket(const ActionToMarket &_data);
        ActionToMarket(const ClientSocket &&_client) noexcept;

        ActionToMarket &operator=(ActionToMarket &&_data) noexcept;

        ClientSocket Client{};                              ///< Sender client information.
        OrderBook::Data OrderData{};                        ///< Action to apply to the OrderBook.
    };

    /// @brief Data transfered from the pip::Market pipeline to the pip::OutNetwork pipeline
    struct MarketToNet
    {
        MarketToNet() = default;
        MarketToNet(const MarketToNet &&_data) noexcept;
        MarketToNet(const MarketToNet &_data);
        MarketToNet(const ClientSocket &&_client, const fix::Message &&_msg) noexcept;
        MarketToNet(const ClientSocket &_client, const fix::Message &_msg);

        MarketToNet &operator=(MarketToNet &&_data) noexcept;

        ClientSocket Client{};                      ///< Sender client information.
        fix::Message Message{};                     ///< Final message send to the client.
    };
}

/// @brief Output data type of the pip::InNetwork pipeline.
using NetOut = data::NetToAction;
/// @brief Input data type of the pip::Action pipeline.
using ActionIn = NetOut;

/// @brief Output data type of the pip::Action pipeline.
using ActionOut = data::ActionToMarket;
/// @brief Input data type of the pip::Market pipeline.
using MarketIn = ActionOut;

/// @brief Output data type of the pip::Market pipeline.
using MarketOut = data::MarketToNet;
/// @brief Input data type of the pip::OutNetwork pipeline.
using NetIn = MarketOut;

/// @brief Input data type of the error to be send with pip::OutNetwork
using ErrorMsg = NetIn;

/// @brief Queue type use to transfer data from pip::InNetwork to pip::Action pipeline.
using NetToAction = ts::Queue<NetOut>;
/// @brief Queue type use to transfer data from pip::Action to pip::Market pipeline.
using ActionToMarket = ts::Queue<ActionOut>;
/// @brief Queue type use to transfer data from pip::Market to pip::OutNetwork pipeline.
using MarketToNet = ts::Queue<MarketOut>;
/// @brief Queue type use to transfer direct message from any pipeline to the pip::OutNetwork pipeline.
using RawOutput = ts::Queue<NetIn>;

/// @brief Queue type use to transfer data to be formated and send by the pip::UDPOutNetwork pipeline.
using UdpInput = ts::Queue<data::UDPPackage>;

/// @brief Map of market input ActionToMarket with as key the symbol of the market.
using MarketEntry = std::unordered_map<std::string, ActionToMarket &>;