#include "Message/Fix.hpp"

namespace fix
{
    std::string Message::checkSum(const std::string &_val) const
    {
        size_t sum = 0;
        std::string formated_checksum = "";

        for (const char &c : _val) {
            sum += static_cast<int>(c);
        }
        sum %= 256;

        if (sum < 10)
            formated_checksum = "00" + std::to_string(sum);
        else if (sum < 100)
            formated_checksum = "0" + std::to_string(sum);
        else
            formated_checksum = std::to_string(sum);
        return formated_checksum;
    }

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
        header.MsgSeqNum = std::to_string(std::stoi(header.MsgSeqNum) + 1);
        header.setBodyLength(header.getPartialHeader() + body_tmp);
        header_tmp = header;

        msg = header_tmp + body_tmp;
        msg += "10=" + checkSum(msg) + (char)FIX_DELIMITER;

        return msg;
    }
}