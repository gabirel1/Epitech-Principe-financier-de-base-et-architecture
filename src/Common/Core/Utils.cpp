#include <numeric>
#include <sstream>

#include "Common/Core/Utils.hpp"

namespace utils
{
    bool is_numeric(const std::string &_str)
    {
        std::string::const_iterator it = _str.begin();

        for (; it != _str.end() && std::isdigit(*it); it++) {}
        return !_str.empty() && it == _str.end();
    }

    bool is_double(const std::string &_str)
    {
        std::istringstream stream(_str);
        double f = 0.f;

        stream >> std::noskipws >> f;
        return stream.eof() && !stream.fail();
    }
}