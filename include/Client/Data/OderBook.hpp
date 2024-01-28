#pragma once

#include "Common/Core/Order.hpp"
#include "Common/Message/Fix.hpp"
#include "Common/Network/UDPPackage.hpp"
#include "Common/Thread/Queue.hpp"

using UDPOutput = ts::Queue<data::UDPPackage>;
using TCPOutput = ts::Queue<fix::Serializer::AnonMessage>;

namespace data
{
    template<class T>
    std::vector<T> extract(fix::Serializer::AnonMessage &_msg, const std::string &_tag, size_t _size);

    struct IncrRefresh
    {
        IncrRefresh(size_t _size);

        size_t size;
        std::vector<Price> prices;
        std::vector<OrderType> types;
        std::vector<Quantity> quantitys;
        std::vector<std::string> symbols;

        bool is_valid() const;
    };

    struct FullRefresh
    {
        size_t size;
        std::string symbol;
        std::vector<OrderType> types;
        std::vector<Price> prices;
        std::vector<Quantity> quantitys;
    };
}

class OderBook
{
    public:
        using BidBook = std::map<Price, Quantity, std::less_equal<Price>>
        using AskBook = std::map<Price, Quantity, std::greater_equal<Price>>

        OrderBook(const std::string &_name, UDPOutput &_udp, TCPOutput &_tcp, TCPInput &_output);
        ~OrderBook();

        bool start();
        bool stop();

        [[nodiscard]] std::shared_ptr<BidBook> getBid(const std::string &_sym);
        [[nodiscard]] std::shared_ptr<AskBook> getAsk(const std::string &_sym);

    protected:
        void loop();

        bool treatFullRefresh(fix::Serializer::AnonMessage &_msg);
        bool treatIncrRefresh(fix::Serializer::AnonMessage &_msg);

        data::IncrRefresh loadIncrRefresh(fix::Serializer::AnonMessage &_msg)
        data::FullRefresh loadFullRefresh(fix::Serializer::AnonMessage &_msg)

        void sendFullRefresh();
        void sendSubscribe();

    private:
        using BidMap = std::map<std::string, std::shared_ptr<BidBook>>;
        using AskMap = std::map<std::string, std::shared_ptr<AskBook>>;
        using SyncFn = Quantity (*)(Quantity, Quantity);

        static Quantity QtySync(Quantity _left, Quantity _right);
        static Quantity CopySync(Quantity _left, Quantity _right);

        const std::string m_name;

        UDPOutput &m_udp;
        TCPOutput &m_tcp;
        TCPInput &m_output;

        bool m_is_sync = false;

        BidMap m_bid;
        AskMap m_ask;

        BidMap m_bid_last;
        AskMap m_ask_last;


        std::thread m_thread;
};

#include "Client/Data/OrderBook.inl"