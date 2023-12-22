#include <numeric>
#include <sstream>

#include "Common/Message/Utils.hpp"

namespace utils
{
    std::size_t getBodyLength(const std::string &_str)
    {
        return _str.size();
    }

    std::string getChecksum(const std::string &_str)
    {
        size_t sum = 0;
        std::string formated_checksum{};

        sum = std::accumulate(_str.begin(), _str.end(), 0);
        sum %= 256;
        if (sum < 10)
            formated_checksum = "00";
        else if (sum < 100)
            formated_checksum = "0";
        formated_checksum += std::to_string(sum);
        return formated_checksum;
    }

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