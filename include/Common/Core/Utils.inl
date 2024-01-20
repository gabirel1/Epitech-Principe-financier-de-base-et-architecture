#include <sstream>

#include "Common/Core/Utils.hpp"
#include "Common/Message/Reject.hpp"
#include <iostream>

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

    template<const char *T, const char *...Ts>
    std::pair<bool, fix::Reject> Has(fix::Serializer::AnonMessage &_msg)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        std::cout << T << std::endl;

        if (!_msg.contains(T)) {
            reject.first = true;
            reject.second.set371_refTagId(T);
            reject.second.set373_sessionRejectReason(fix::Reject::ReqTagMissing);
            reject.second.set58_text("Unable to find required field");
        } else if (_msg.at(T).empty()) {
            std::cout << "EMPTY => " << T << std::endl;
            reject.first = true;
            reject.second.set371_refTagId(T);
            reject.second.set373_sessionRejectReason(fix::Reject::EmptyValue);
            reject.second.set58_text("Waiting a value");
        } else {
            if constexpr (sizeof...(Ts) == 0)
                return { false, fix::Reject{} };
            else
                return utils::Has<Ts...>(_msg);
        }
        return reject;
    }
}