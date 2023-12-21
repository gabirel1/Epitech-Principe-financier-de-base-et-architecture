#include "Message/Fix.hpp"
#include <numeric>
#include <iostream>
#include "Utils/Utils.hpp"

namespace fix
{
    // Message::operator std::string () const
    // {
    //     std::string str = header;

    //     for (const auto &[_key, _val] : m_params)
    //         str += _key + "=" + _val + "^";
    //     str += "10" + checkSum(str) + "^";
    //     return str;
    // }

    Message::operator std::string ()
    {
        std::string body_tmp = "";
        std::string header_tmp = "";
        std::string msg = "";

        for (const auto &[_key, _val] : m_params)
            body_tmp += _key + "=" + _val + (char)FIX_DELIMITER;

        header.setSendingTime();
        // header.updateMsgSeqNum();

        std::size_t _len = Utils::getBodyLength(
            header.getPartialHeader() + body_tmp
        );

        header.setBodyLength(_len);
        header_tmp = header;

        msg = header_tmp + body_tmp;
        msg += "10=" + Utils::getChecksum(msg) + (char)FIX_DELIMITER;

        return msg;
    }
}