#include <sstream>

#include "Common/Message/Utils.hpp"

namespace utils
{
    template<class T>
    T to(const std::string &_str)
    {
        T val;
        std::istringstream stream(_str);

        stream >> val;
        return val;
    }
}