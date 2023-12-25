#pragma once

namespace fix
{
    /// @brief Structure containing all the [Tag](https://www.onixs.biz/fix-dictionary/4.2/fields_by_tag.html) of the FIX protocol.
    struct Tag
    {
        static constexpr const char BeginString[] = "8";            ///< Tag [8](https://www.onixs.biz/fix-dictionary/4.2/tagNum_8.html).
        static constexpr const char BodyLength[] = "9";             ///< Tag [9](https://www.onixs.biz/fix-dictionary/4.2/tagNum_9.html).
        static constexpr const char ClOrdID[] = "11";;              ///< Tag [11](https://www.onixs.biz/fix-dictionary/4.2/tagNum_11.html).
        static constexpr const char HandlInst[] = "21";             ///< Tag [21](https://www.onixs.biz/fix-dictionary/4.2/tagNum_21.html).
        static constexpr const char MsqSeqNum[] = "34";             ///< Tag [34](https://www.onixs.biz/fix-dictionary/4.2/tagNum_34.html).
        static constexpr const char MsgType[] = "35";               ///< Tag [35](https://www.onixs.biz/fix-dictionary/4.2/tagNum_35.html).
        static constexpr const char OrderQty[] = "38";              ///< Tag [38](https://www.onixs.biz/fix-dictionary/4.2/tagNum_38.html).
        static constexpr const char OrdType[] = "40";               ///< Tag [40](https://www.onixs.biz/fix-dictionary/4.2/tagNum_40.html).
        static constexpr const char OrigClOrdID[] = "41";           ///< Tag [41](https://www.onixs.biz/fix-dictionary/4.2/tagNum_41.html).
        static constexpr const char Price[] = "44";                 ///< Tag [44](https://www.onixs.biz/fix-dictionary/4.2/tagNum_44.html).
        static constexpr const char RefSeqNum[] = "45";             ///< Tag [45](https://www.onixs.biz/fix-dictionary/4.2/tagNum_45.html).
        static constexpr const char SenderCompId[] = "49";          ///< Tag [49](https://www.onixs.biz/fix-dictionary/4.2/tagNum_49.html).
        static constexpr const char SendingTime[] = "52";           ///< Tag [52](https://www.onixs.biz/fix-dictionary/4.2/tagNum_52.html).
        static constexpr const char Side[] = "54";                  ///< Tag [54](https://www.onixs.biz/fix-dictionary/4.2/tagNum_54.html).
        static constexpr const char Symbol[] = "55";                ///< Tag [55](https://www.onixs.biz/fix-dictionary/4.2/tagNum_55.html).
        static constexpr const char TargetCompId[] = "56";          ///< Tag [56](https://www.onixs.biz/fix-dictionary/4.2/tagNum_56.html).
        static constexpr const char Text[] = "58";                  ///< Tag [58](https://www.onixs.biz/fix-dictionary/4.2/tagNum_58.html).
        static constexpr const char TransactTime[] = "60";          ///< Tag [60](https://www.onixs.biz/fix-dictionary/4.2/tagNum_60.html).
        static constexpr const char EncryptMethod[] = "98";         ///< Tag [98](https://www.onixs.biz/fix-dictionary/4.2/tagNum_98.html).
        static constexpr const char HearBtInt[] = "108";            ///< Tag [108](https://www.onixs.biz/fix-dictionary/4.2/tagNum_108.html).
        static constexpr const char TestReqId[] = "112";            ///< Tag [112](https://www.onixs.biz/fix-dictionary/4.2/tagNum_112.html).
        static constexpr const char RefTagId[] = "371";             ///< Tag [112](https://www.onixs.biz/fix-dictionary/4.2/tagNum_371.html).
        static constexpr const char RefMsgType[] = "352";           ///< Tag [112](https://www.onixs.biz/fix-dictionary/4.2/tagNum_352.html).
        static constexpr const char SessionRejectReason[] = "373";  ///< Tag [112](https://www.onixs.biz/fix-dictionary/4.2/tagNum_373.html).
    };
}