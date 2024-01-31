#pragma once

#include <memory>
#include <map>
#include <optional>

#include "Common/Core/Utils.hpp"
#include "Common/Network/UDPPackage.hpp"
#include "Common/Thread/Queue.hpp"
#include "Common/Message/Fix.hpp"
#include "Client/Processor/IProcessor.hpp"

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

namespace proc
{
    class OrderBook : public proc::IMessage, public proc::IUDP
    {
        public:
            using BidBook = std::map<Price, Quantity, std::less_equal<Price>>;
            using AskBook = std::map<Price, Quantity, std::greater_equal<Price>>;

            OrderBook() = default;
            ~OrderBook() = default;

            std::optional<fix::Message> process(fix::Serializer::AnonMessage &_msg, Context &_ctx) override final;
            std::optional<data::UDPPackage> process(const data::UDPPackage &_package, Context &_ctx) override final;

            virtual std::optional<fix::Message> build(char _tag, Context &_ctx) const override final;

        protected:
            void treatIncrRefresh(fix::Serializer::AnonMessage &_msg);
            void treatFullRefresh(fix::Serializer::AnonMessage &_msg);

            data::IncrRefresh loadIncrRefresh(fix::Serializer::AnonMessage &_msg);
            data::FullRefresh loadFullRefresh(fix::Serializer::AnonMessage &_msg);

            fix::Message buildFullRefresh() const;
            fix::Message buildIncrRefresh() const;

        private:
            using BidMap = std::map<std::string, BidBook>;
            using AskMap = std::map<std::string, AskBook>;
            using SyncFn = Quantity (*)(Quantity, Quantity);

            static Quantity QtySync(Quantity _left, Quantity _right);
            static Quantity CopySync(Quantity _left, Quantity _right);

            template<class T, class _T>
            static void sync_book(T &_origine, _T &_target, SyncFn _sync)
            {
                for (auto [_sym, _book] : _origine)
                    for (auto &[_price, _qty] : _book)
                        functional_sync<T>(_target, _sym, _price, _qty, _sync);
            }

            // turn this function into class later
            template<class T>
            static bool functional_sync(T &_map, const std::string &_sym, Price _price, Quantity _qty, SyncFn _sync)
            {
                if (_qty != 0)
                    _map[_sym][_price] = _sync(_qty, _map[_sym][_price]);
                else
                    _map[_sym].erase(_price);
                return true;
            }

            bool m_running = false;

            bool m_is_sync = false;

            BidMap m_bid;
            AskMap m_ask;

            BidMap m_bid_last;
            AskMap m_ask_last;
    };
}

#include "Client/Processor/OrderBook.inl"