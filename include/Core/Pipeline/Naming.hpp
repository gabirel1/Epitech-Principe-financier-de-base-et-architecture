#pragma once

#include "Thread/Queue.hpp"
#include "Core/OrderBook.hpp"
#include "Message/Fix.hpp"

using NetOut = int; // todo
using SerialIn = NetOut;

using SerialOut = OrderBook::Data;
using MarketIn = SerialOut;

using MarketOut = int; // todo
using NetIn = MarketOut;

using NetToSerial = ts::Queue<NetOut>;
using SerialToMarket = ts::Queue<SerialOut>;
using MarketToNet = ts::Queue<MarketOut>;
using RawOutput = ts::Queue<fix::Message>;