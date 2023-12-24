#include <numeric>
#include <iostream>
#include "Common/Message/Utils.hpp"
#include "Common/Message/Fix.hpp"

namespace fix
{
    Message::operator std::string ()
    {
        std::string body_tmp = "";
        std::string header_tmp = "";
        std::string msg = "";

        for (const auto &[_key, _val] : m_params)
            body_tmp += _key + "=" + _val + (char)FIX_DELIMITER;

        header.setSendingTime();
        // header.updateMsgSeqNum();

        std::size_t _len = utils::getBodyLength(
            header.getPartialHeader() + body_tmp
        );

        header.set9_bodyLength(std::to_string(_len));
        header_tmp = header;

        msg = header_tmp + body_tmp;
        msg += "10=" + utils::getChecksum(msg) + (char)FIX_DELIMITER;

        return msg;
    }

    std::string Message::to_string()
    {
        std::string body_tmp = "";
        std::string header_tmp = "";
        std::string msg = "";

        for (const auto &[_key, _val] : m_params)
            body_tmp += _key + "=" + _val + (char)FIX_DELIMITER;

        header.setSendingTime();
        // header.updateMsgSeqNum();

        std::size_t _len = utils::getBodyLength(
            header.getPartialHeader() + body_tmp
        );

        header.set9_bodyLength(std::to_string(_len));
        header_tmp = header;

        msg = header_tmp + body_tmp;
        msg += "10=" + utils::getChecksum(msg) + (char)FIX_DELIMITER;

        return msg;
    }
}