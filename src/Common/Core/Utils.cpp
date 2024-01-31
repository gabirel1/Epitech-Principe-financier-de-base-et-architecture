#include <numeric>
#include <sstream>

#include "Common/Core/Utils.hpp"
#include <random>

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

    std::string id() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9999999);

        unsigned int id = dis(gen);
        std::string idStr = std::to_string(id);

        while (idStr.length() < 7) {
            idStr = "0" + idStr;
        }

        return idStr;
    }

}