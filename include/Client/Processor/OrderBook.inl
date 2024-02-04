#include <algorithm>

#include "Client/Processor/OrderBook.hpp"

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
}

namespace proc
{
    template<class T, class _T>
    void OrderBook::sync_book(T &_origine, _T &_target, SyncFn _sync)
    {
        for (auto [_sym, _book] : _origine)
            for (auto &[_price, _qty] : _book)
                functional_sync<T>(_target, _sym, _price, _qty, _sync);
    }

    // turn this function into class later
    template<class T>
    bool OrderBook::functional_sync(T &_map, const std::string &_sym, Price _price, Quantity _qty, SyncFn _sync)
    {
        if (_qty != 0)
            _map[_sym][_price] = _sync(_qty, _map[_sym][_price]);
        else
            _map[_sym].erase(_price);
        return true;
    }
}