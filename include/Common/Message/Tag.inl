#include "Common/Message/Tag.hpp"
#include "Common/Core/Utils.hpp"

namespace fix
{
    template<const char *T, const char *...Ts>
    std::pair<bool, Reject> verify_all(Serializer::AnonMessage &_msg)
    {
        std::pair<bool, fix::Reject> reject = verify<T>(_msg.at(T));

        if (reject.first)
            return reject;
        if constexpr (sizeof...(Ts) == 0)
            return reject;
        else
            return verify_all<Ts...>(_msg);
    }

    template<const char *T>
    std::pair<bool, Reject> verify(const std::string &_value)
    {
        // the definition of the specialization are made after the static_assert(false), so it need to be calculated to have the instance of the specialization
        static_assert(std::is_same_v<bool, float>, "No specialization for the verification of this tag");
        return { true,  {} };
    }

    template<>
    std::pair<bool, Reject> verify<Tag::EncryptMethod>(const std::string &_value)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        if (_value != "0") {
            reject.first = true;
            reject.second.set371_refTagId(Tag::EncryptMethod);
            reject.second.set373_sessionRejectReason(Reject::DecryptionIssue);
            reject.second.set58_text("Not supported encryption method");
        }
        return reject;
    }

    template<>
    std::pair<bool, Reject> verify<Tag::HearBtInt>(const std::string &_value)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        if (!utils::is_numeric(_value)) {
            reject.first = true;
            reject.second.set371_refTagId(Tag::HearBtInt);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Heart beat should be a numerical value");
        }
        return reject;
    }

    template<>
    std::pair<bool, Reject> verify<Tag::ClOrdID>(const std::string &_value)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        if (!utils::is_numeric(_value)) {
            reject.first = true;
            reject.second.set371_refTagId(Tag::ClOrdID);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Not supported order Id");
        }
        return reject;
    }

    template<>
    std::pair<bool, Reject> verify<Tag::HandlInst>(const std::string &_value)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        if (_value != "3") {
            reject.first = true;
            reject.second.set371_refTagId(Tag::HandlInst);
            reject.second.set373_sessionRejectReason(Reject::ValueOORange);
            reject.second.set58_text("Not supported order Id");
        }
        return reject;
    }

    template<>
    std::pair<bool, Reject> verify<Tag::OrderQty>(const std::string &_value)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        if (!utils::is_double(_value)) {
            reject.first = true;
            reject.second.set371_refTagId(Tag::OrderQty);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Quantity should be a double");
        }
        return reject;
    }

    template<>
    std::pair<bool, Reject> verify<Tag::OrdType>(const std::string &_value)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        if (_value != "2") {
            reject.first = true;
            reject.second.set371_refTagId(Tag::OrdType);
            reject.second.set373_sessionRejectReason(Reject::ValueOORange);
            reject.second.set58_text("Order type not supported");
        }
        return reject;
    }

    template<>
    std::pair<bool, Reject> verify<Tag::Price>(const std::string &_value)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        if (!utils::is_double(_value)) {
            reject.first = true;
            reject.second.set371_refTagId(Tag::Price);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Price should be a double");
        }
        return reject;
    }

    template<>
    std::pair<bool, Reject> verify<Tag::Side>(const std::string &_value)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        if (_value != "3" && _value != "4") {
            reject.first = true;
            reject.second.set371_refTagId(Tag::Side);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Value not supported (only 3 or 4 are allowed)");
        }
        return reject;
    }

    template<>
    std::pair<bool, Reject> verify<Tag::TransactTime>(const std::string &_value)
    {
        std::ignore = _value;

        return { false, {} };
    }

    template<>
    std::pair<bool, Reject> verify<Tag::Symbol>(const std::string &_value)
    {
        std::ignore = _value;

        return { false, {} };
    }

    template<>
    std::pair<bool, Reject> verify<Tag::OrigClOrdID>(const std::string &_value)
    {
        std::pair<bool, fix::Reject> reject = { false, {} };

        if (!utils::is_numeric(_value)) {
            reject.first = true;
            reject.second.set371_refTagId(Tag::OrigClOrdID);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("The origin Id should be numerical");
        }
        return reject;
    }
}
