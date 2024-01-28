#pragma once

#include <memory>
#include <map>

#include "Common/Core/Utils.hpp"
#include "Common/Message/Fix.hpp"
#include "Common/Network/UDPPackage.hpp"
#include "Common/Thread/Queue.hpp"

using UDPOutput = ts::Queue<data::UDPPackage>;
using TCPOutput = ts::Queue<fix::Serializer::AnonMessage>;
using TCPInput = ts::Queue<fix::Message>;

namespace data
{
    template<class T>
    std::vector<T> extract(fix::Serializer::AnonMessage &_msg, const std::string &_tag)
    {
        std::vector<std::string> list = utils::split<','>(_msg.at(_tag));
        std::vector<T> result(list.size());

        std::for_each(list.begin(), list.end(), [&result] (const std::string &_value) {
            result.emplace_back(utils::to<T>(_value));
        });
        return result;
    }

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

class OrderBook
{
    public:
        using BidBook = std::map<Price, Quantity, std::less_equal<Price>>;
        using AskBook = std::map<Price, Quantity, std::greater_equal<Price>>;

        OrderBook(const std::string &_name, UDPOutput &_udp, TCPOutput &_tcp, TCPInput &_output);
        ~OrderBook();

        bool start();
        bool stop();

        [[nodiscard]] std::shared_ptr<BidBook> getBid(const std::string &_sym);
        [[nodiscard]] std::shared_ptr<AskBook> getAsk(const std::string &_sym);

    protected:
        void loop();

        void treatFullRefresh(fix::Serializer::AnonMessage &_msg);
        void treatIncrRefresh(fix::Serializer::AnonMessage &_msg);

        data::IncrRefresh loadIncrRefresh(fix::Serializer::AnonMessage &_msg);
        data::FullRefresh loadFullRefresh(fix::Serializer::AnonMessage &_msg);

        void sendFullRefresh();
        void sendSubscribe();

    private:
        using BidMap = std::map<std::string, std::shared_ptr<BidBook>>;
        using AskMap = std::map<std::string, std::shared_ptr<AskBook>>;
        using SyncFn = Quantity (*)(Quantity, Quantity);

        static Quantity QtySync(Quantity _left, Quantity _right);
        static Quantity CopySync(Quantity _left, Quantity _right);

        template<class T, class _T>
        static void sync_book(T &_origine, _T &_target, SyncFn _sync)
        {
            for (auto [_sym, _book] : _origine)
                for (auto &[_price, _qty] : *_book)
                    functional_sync<T>(_target, _sym, _price, _qty, _sync);
        }

        // turn this function into class later
        template<class T>
        static bool functional_sync(T &_map, const std::string &_sym, Price _price, Quantity _qty, SyncFn _sync)
        {
            if (_map.find(_sym) == _map.end())
                _map[_sym] = std::make_shared<typename T::mapped_type::element_type>();
            if (_qty != 0)
                (*_map[_sym])[_price] = _sync(_qty, (*_map[_sym])[_price]);
            else
                _map[_sym]->erase(_price);
            return true;
        }

        const std::string m_name;

        bool m_running = false;

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