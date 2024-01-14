#pragma once

#include <utility>

#include "Common/Message/Reject.hpp"

#define UNUSED __attribute__((unused))

namespace fix
{
    /// @brief Structure containing all the [Tag](https://www.onixs.biz/fix-dictionary/4.2/fields_by_tag.html) of the FIX protocol.
    namespace Tag
    {
        constexpr const char BeginString[] = "8";            ///< Tag [8](https://www.onixs.biz/fix-dictionary/4.2/tagNum_8.html).
        constexpr const char BodyLength[] = "9";             ///< Tag [9](https://www.onixs.biz/fix-dictionary/4.2/tagNum_9.html).
        constexpr const char ClOrdID[] = "11";;              ///< Tag [11](https://www.onixs.biz/fix-dictionary/4.2/tagNum_11.html).
        constexpr const char HandlInst[] = "21";             ///< Tag [21](https://www.onixs.biz/fix-dictionary/4.2/tagNum_21.html).
        constexpr const char MsqSeqNum[] = "34";             ///< Tag [34](https://www.onixs.biz/fix-dictionary/4.2/tagNum_34.html).
        constexpr const char MsgType[] = "35";               ///< Tag [35](https://www.onixs.biz/fix-dictionary/4.2/tagNum_35.html).
        constexpr const char OrderQty[] = "38";              ///< Tag [38](https://www.onixs.biz/fix-dictionary/4.2/tagNum_38.html).
        constexpr const char OrdType[] = "40";               ///< Tag [40](https://www.onixs.biz/fix-dictionary/4.2/tagNum_40.html).
        constexpr const char OrigClOrdID[] = "41";           ///< Tag [41](https://www.onixs.biz/fix-dictionary/4.2/tagNum_41.html).
        constexpr const char Price[] = "44";                 ///< Tag [44](https://www.onixs.biz/fix-dictionary/4.2/tagNum_44.html).
        constexpr const char RefSeqNum[] = "45";             ///< Tag [45](https://www.onixs.biz/fix-dictionary/4.2/tagNum_45.html).
        constexpr const char SenderCompId[] = "49";          ///< Tag [49](https://www.onixs.biz/fix-dictionary/4.2/tagNum_49.html).
        constexpr const char SendingTime[] = "52";           ///< Tag [52](https://www.onixs.biz/fix-dictionary/4.2/tagNum_52.html).
        constexpr const char Side[] = "54";                  ///< Tag [54](https://www.onixs.biz/fix-dictionary/4.2/tagNum_54.html).
        constexpr const char Symbol[] = "55";                ///< Tag [55](https://www.onixs.biz/fix-dictionary/4.2/tagNum_55.html).
        constexpr const char TargetCompId[] = "56";          ///< Tag [56](https://www.onixs.biz/fix-dictionary/4.2/tagNum_56.html).
        constexpr const char Text[] = "58";                  ///< Tag [58](https://www.onixs.biz/fix-dictionary/4.2/tagNum_58.html).
        constexpr const char TransactTime[] = "60";          ///< Tag [60](https://www.onixs.biz/fix-dictionary/4.2/tagNum_60.html).
        constexpr const char EncryptMethod[] = "98";         ///< Tag [98](https://www.onixs.biz/fix-dictionary/4.2/tagNum_98.html).
        constexpr const char HearBtInt[] = "108";            ///< Tag [108](https://www.onixs.biz/fix-dictionary/4.2/tagNum_108.html).
        constexpr const char TestReqId[] = "112";            ///< Tag [112](https://www.onixs.biz/fix-dictionary/4.2/tagNum_112.html).
        constexpr const char RefTagId[] = "371";             ///< Tag [112](https://www.onixs.biz/fix-dictionary/4.2/tagNum_371.html).
        constexpr const char RefMsgType[] = "352";           ///< Tag [112](https://www.onixs.biz/fix-dictionary/4.2/tagNum_352.html).
        constexpr const char SessionRejectReason[] = "373";  ///< Tag [112](https://www.onixs.biz/fix-dictionary/4.2/tagNum_373.html).
        constexpr const char CheckSum[] = "10";              ///< Tag [10](https://www.onixs.biz/fix-dictionary/4.2/tagNum_10.html).

    };

    /// @brief Verify the value of all the fix::Tag passed as template.
    /// @tparam T fix::Tag to verify in the function.
    /// @tparam ...Ts Remaining of fix::Tag to check recursivly.
    /// @param _msg Map of the message.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<const char *T, const char *...Ts>
    std::pair<bool, Reject> verify_all(Serializer::AnonMessage &_msg);


    template<const char *T>
    std::pair<bool, Reject> verify(const std::string &_value);

    /// @brief Verify the value of Tag::EncryptMethod.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    UNUSED std::pair<bool, Reject> verify<Tag::EncryptMethod>(const std::string &_value);
    /// @brief Verify the value of Tag::HearBtInt.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    UNUSED std::pair<bool, Reject> verify<Tag::HearBtInt>(const std::string &_value);
    /// @brief Verify the value of Tag::ClOrdID.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    UNUSED std::pair<bool, Reject> verify<Tag::ClOrdID>(const std::string &_value);
    /// @brief Verify the value of Tag::HandlInst.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    UNUSED std::pair<bool, Reject> verify<Tag::HandlInst>(const std::string &_value);
    /// @brief Verify the value of Tag::OrderQty.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    UNUSED std::pair<bool, Reject> verify<Tag::OrderQty>(const std::string &_value);
    /// @brief Verify the value of Tag::OrdType.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    UNUSED std::pair<bool, Reject> verify<Tag::OrdType>(const std::string &_value);
    /// @brief Verify the value of Tag::Price.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    UNUSED std::pair<bool, Reject> verify<Tag::Price>(const std::string &_value);
    /// @brief Verify the value of Tag::Side.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    UNUSED std::pair<bool, Reject> verify<Tag::Side>(const std::string &_value);
    /// @brief Verify the value of Tag::TransactTime.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    [[deprecated("Not implemented yet")]]
    UNUSED std::pair<bool, Reject> verify<Tag::TransactTime>(const std::string &_value);
    /// @brief Verify the value of Tag::Symbol.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    [[deprecated("Not implemented yet")]]
    UNUSED std::pair<bool, Reject> verify<Tag::Symbol>(const std::string &_value);
    /// @brief Verify the value of Tag::OrigClOrdID.
    /// @param _value value of the tag.
    /// @return If the first element is true then second is set, otherwise it rigly formated.
    template<>
    UNUSED std::pair<bool, Reject> verify<Tag::OrigClOrdID>(const std::string &_value);

#pragma GCC diagnostic pop

}

#include "Common/Message/Tag.inl"