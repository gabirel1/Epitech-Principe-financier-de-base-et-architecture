#pragma once

#include <string>

namespace utils
{
    bool is_numeric(const std::string &_str);
    bool is_double(const std::string &_str);

    template<class T>
    T to(const std::string &_str);
}

#include "Common/Core/Utils.inl"