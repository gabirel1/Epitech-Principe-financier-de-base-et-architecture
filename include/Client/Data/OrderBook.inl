#include <algorithm>

#include "Client/Data/OrderBook.hpp"

namespace data
{
    template<class T>
    std::vector<T> extract(fix::Serializer::AnonMessage &_msg, const std::string &_tag, size_t _size)
    {
        std::<std::string> list = utils::split<','>(_msg.to_string());
        std::vector<T> result{list.size()};

        std::for_each(list.begin(), list.end(), [] (const std::string &_value) {
            result.push_back(utils::to<T>(_value));
        });
        return result;
    }
}