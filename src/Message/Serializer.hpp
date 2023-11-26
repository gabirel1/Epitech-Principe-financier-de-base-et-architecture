#include <algorithm>

#include "Message/Serializer.hpp"

namespace fix
{
    Serializer::Error Serializer::run(std::string &_msg, AnonMessage &_map)
    {
        AnonPair pair;

        _map.clear();
        while (!_msg.empty()) {
            Error err = token(_msg, pair);

            if (err == Error::None)
                return err;
            _map.emplace_back(std::move(pair));
        }
        return Error::None;
    }

    Serializer::Error Serializer::token(std::string &_msg, AnonPair &_pair)
    {
        size_t size = _msg.find("*");

        if (size == std::string::npos)
            return Error::InvalidEnd;
        size_t split = _msg.find("=");

        if (split < size)
            return Error::NoEqual;
        std::string key = _msg.substr(0, split);

        if (key.empty() && std::all_of(key.begin(), key.end(), ::isdigit))
            return Error::InvalidKey;
        _pair = { std::move(key), std::move(_msg.substr(split + 1, size)) };
        _msg.erase(0, size);
        return Error::None;
    }
}