#include <algorithm>

#include "Common/Message/Serializer.hpp"
#include <iostream>

namespace fix
{
    Serializer::Error Serializer::run(std::string &_msg, AnonMessage &_map)
    {
        Token pair;

        _map.clear();
        while (!_msg.empty()) {
            std::cout << "msg -| '" << _msg << "'" << std::endl;
            std::string tmp = _msg;
            std::replace(tmp.begin(), tmp.end(), (char)FIX_DELIMITER, '^');
            std::cout << "msg.replace -| '" << tmp  << "'" << std::endl;
            Error err = token(_msg, pair);
            std::cout << "Error: " << (int)err << std::endl;
            std::cout << std::endl << std::endl;
            if (err != Error::None)
                return err;
            _map.emplace(std::move(pair));
        }
        return Error::None;
    }

    Serializer::Error Serializer::token(std::string &_msg, Token &_pair)
    {
        size_t size = _msg.find((char)FIX_DELIMITER);
        std::cout << "Size: " << size << std::endl;

        if (size == std::string::npos)
            return Error::InvalidEnd;
        size_t split = _msg.find("=");

        std::cout << "Split: " << split << std::endl;
        if (split >= size)
            return Error::NoEqual;
        std::string key = _msg.substr(0, split);

        if (key.empty() && std::all_of(key.begin(), key.end(), ::isdigit))
            return Error::InvalidKey;
        _pair = { std::move(key), std::move(_msg.substr(split + 1, size - (key.size() + 1))) };
        std::cout << "Key: '" << _pair.first << "'" << std::endl;
        std::cout << "Value: '" << _pair.second << "'" << std::endl;

        std::string tmp = _pair.second;
        std::replace(tmp.begin(), tmp.end(), (char)FIX_DELIMITER, '^');

        std::cout << "Valuetmp: '" << tmp << "'" << std::endl;
        std::cout << "value[8] '" << tmp[tmp.size()] << "'" << std::endl;
        std::cout << "value size: " << _pair.second.size() << std::endl;
        std::cout << "value size: " << tmp.size() << std::endl;
        _msg.erase(0, size + 1);
        return Error::None;
    }

    std::string Serializer::serialize(const AnonMessage &_map)
    {
        std::string msg;

        for (const auto &[key, val] : _map)
            msg += key + "=" + val + (char)FIX_DELIMITER;
        return msg;
    }
}