#include <numeric>

#include "Common/Message/Fix.hpp"
#include "Common/Core/Utils.hpp"
#include "Common/Message/Tag.hpp"
#include <iostream>

namespace fix
{
    Message::operator std::string()
    {
        return to_string();
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

        std::size_t _len = Message::getBodyLength(
            header.getPartialHeader() + body_tmp
        );

        header.set9_bodyLength(std::to_string(_len));
        header_tmp = header;

        msg = header_tmp + body_tmp;
        msg += "10=" + Message::getChecksum(msg) + (char)FIX_DELIMITER;

        return msg;
    }

    std::size_t Message::getBodyLength(const std::string &_str)
    {
        return _str.size();
    }

    std::string Message::getChecksum(const std::string &_str)
    {
        size_t sum = 0;
        std::string formated_checksum{};

        sum = std::accumulate(_str.begin(), _str.end(), 0) % 256;
        if (sum < 10)
            formated_checksum = "00";
        else if (sum < 100)
            formated_checksum = "0";
        formated_checksum += std::to_string(sum);
        return formated_checksum;
    }

    static std::pair<bool, Reject> VerifyValid(Serializer::AnonMessage &_msg)
    {
        std::pair<bool, Reject> reject = { false, {} };

        std::size_t supposed_length = utils::to<int>(_msg.at(Tag::BodyLength));
        std::string supposed_checksum = _msg.at(Tag::CheckSum);

        std::string msg_body_length = "";
        std::string msg_checksum = "";

        for (const auto &[_key, _val] : _msg)
        {
            if (_key == Tag::CheckSum)
                continue;
            if (_key != Tag::BodyLength && _key != Tag::BeginString)
                msg_body_length += (_key + "=" + _val + (char)FIX_DELIMITER);
            msg_checksum += (_key + "=" + _val + (char)FIX_DELIMITER);
        }

        std::cout << "supposed_length: " << supposed_length << std::endl;
        std::cout << "supposed_checksum: " << supposed_checksum << std::endl;

        if (supposed_length != Message::getBodyLength(msg_body_length)) {
            reject.first = true;
            reject.second.set371_refTagId(Tag::BodyLength);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Body length is not correct");
        } else if (supposed_checksum != Message::getChecksum(msg_checksum)) {
            reject.first = true;
            reject.second.set371_refTagId(Tag::CheckSum);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Checksum is not correct");
        } else {
            reject.first = false;
        }
        return reject;
    }

}