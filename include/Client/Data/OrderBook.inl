// #include <algorithm>

// #include "Client/Data/OrderBook.hpp"

// namespace data
// {
//     template<class T>
//     std::vector<T> extract(fix::Serializer::AnonMessage &_msg, const std::string &_tag, size_t _size)
//     {
//         std::<std::string> list = utils::split<','>(_msg.to_string());
//         std::vector<T> result{list.size()};

//         std::for_each(list.begin(), list.end(), [] (const std::string &_value) {
//             result.push_back(utils::to<T>(_value));
//         });
//         return result;
//     }
// }

// template<class T, class _T>
// void OrderBook::sync_book(T &_origine, _T &_target, SyncFn _sync)
// {
//     for (auto [_sym, _book] : _origine) {
//         for (auto [_price, _qty] : _book) {
//             _target[_symbol][_price] = _sync(_target[_symbol][_price], _qty);
//             if (_target[_symbol][_price] <= 0)
//                 _target[_symbol].erase(_price);
//         }
//     }
// }