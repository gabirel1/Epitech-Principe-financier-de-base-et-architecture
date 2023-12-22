#pragma once

#include <string>

namespace utils
{
    std::size_t getBodyLength(const std::string &_str);
    std::string getChecksum(const std::string &_str);

    bool is_numeric(const std::string &_str);
    bool is_double(const std::string &_str);

    template<class T>
    T to(const std::string &_str);
}

#include "Common/Message/Utils.inl"
