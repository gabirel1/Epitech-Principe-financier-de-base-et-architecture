#include "Common/Core/Utils.hpp"
#include "Common/Message/Logon.hpp"
#include "Common/Message/Tag.hpp"

namespace fix
{
    Logon::Logon()
    {
        header.set35_MsgType(MsgType);
    }

    std::pair<bool, Reject> Logon::Verify(Serializer::AnonMessage &_msg)
    {
        std::pair<bool, Reject> reject = utils::Has<Tag::EncryptMethod, Tag::HearBtInt>(_msg);

        reject.second.set372_refMsgType(Logon::MsgType);
        if (reject.first) {
            return reject;
        } else if (_msg.at(Tag::EncryptMethod) != "0") {
            reject.second.set371_refTagId(Tag::EncryptMethod);
            reject.second.set373_sessionRejectReason(Reject::DecryptionIssue);
            reject.second.set58_text("Not supported encryption method");
        } else if (!utils::is_numeric(_msg.at(Tag::HearBtInt))) {
            reject.second.set371_refTagId(Tag::HearBtInt);
            reject.second.set373_sessionRejectReason("6");
            reject.second.set58_text("Should be a numeric value");
        } else {
            reject.first = false;
        }
        return reject;
    }

    void Logon::set98_EncryptMethod(const std::string &_val)
    {
        m_params.emplace({ Tag::EncryptMethod, _val });
    }

    void Logon::set108_HeartBtInt(const std::string &_val)
    {
        m_params.emplace({ Tag::HearBtInt, _val });
    }
}