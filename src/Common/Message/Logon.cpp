#include "Common/Message/Logon.hpp"
#include "Common/Message/Utils.hpp"

namespace fix
{
    Logon::Logon()
    {
        header.set35_MsgType(m_msgType);
    }

    std::pair<bool, Reject> Logon::Verify(Serializer::AnonMessage &_msg)
    {
        std::pair<bool, Reject> reject{ true, {} };

        reject.second.set372_refMsgType(m_msgType);
        if (!_msg.contains("98") || !_msg.contains("108")) {
            if (!_msg.contains("98"))
                reject.second.set371_refTagId("98");
            else
                reject.second.set371_refTagId("108");
            reject.second.set373_sessionRejectReason("1");
            reject.second.set58_Text("Unable to find required field");
        } else if (_msg.at("98") != "0") {
            reject.second.set371_refTagId("98");
            reject.second.set373_sessionRejectReason("7");
            reject.second.set58_Text("Not supported encryption method");
        } else if (!utils::is_numeric(_msg.at("108"))) {
            reject.second.set371_refTagId("108");
            reject.second.set373_sessionRejectReason("6");
            reject.second.set58_Text("Not supported encryption method");
        } else {
            reject.first = false;
        }
        return reject;
    }

    void Logon::set108_HeartBtInt(const std::string &_val)
    {
        m_params.emplace({ "108", _val });
    }

    void Logon::set98_EncryptMethod(const std::string &_val)
    {
        m_params.emplace({ "98", _val });
    }
}