#include "Common/Message/Utils.hpp"
#include <numeric>

std::size_t Utils::getBodyLength(const std::string &_str)
{
    return _str.size();
}

std::string Utils::getChecksum(const std::string &_str)
{
    size_t sum = 0;
    std::string formated_checksum = "";

    sum = std::accumulate(_str.begin(), _str.end(), 0);
    sum %= 256;

    if (sum < 10)
        formated_checksum = "00";
    else if (sum < 100)
        formated_checksum = "0";
    formated_checksum += std::to_string(sum);

    return formated_checksum;
}