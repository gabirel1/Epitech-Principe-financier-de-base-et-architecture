#include "Common/Message/Fix.hpp"
#include "Common/Message/Reject.hpp"

namespace fix
{
    template<const char *T, const char *...Ts>
    std::pair<bool, Reject> Message::Has(Serializer::AnonMessage &_msg)
    {
        std::pair<bool, Reject> reject = Has<T>(_msg);

        if (reject.first)
            return reject;
        return Has<Ts...>(_msg);
    }

    template<const char *T>
    std::pair<bool, Reject> Message::Has(Serializer::AnonMessage &_msg)
    {
        std::pair<bool, Reject> reject = { false, {} };

        if (!_msg.contains(T)) {
            reject.first = true;
            reject.second.set373_sessionRejectReason(Reject::ReqTagMissing);
            reject.second.set58_text("Unable to find required field");
        } else if (_msg.at(T).empty()) {
            reject.first = true;
            reject.second.set371_refTagId(T);
            reject.second.set373_sessionRejectReason(Reject::EmptyValue);
            reject.second.set58_text("Waiting a value");
        }
        return reject;
    }
}