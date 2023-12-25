#include <iomanip>
#include <ctime>
#include <iostream>

#include <sys/time.h>

#include "Common/Core/Utils.hpp"
#include "Common/Message/Header.hpp"
#include "Common/Message/Tag.hpp"

namespace fix
{
    std::pair<bool, Reject> Header::Verify(Serializer::AnonMessage &_msg)
    {
        // need to verify sending time
        std::pair<bool, Reject> reject = utils::Has<Tag::BeginString, Tag::BodyLength, Tag::MsqSeqNum, Tag::MsgType, Tag::SenderCompId, Tag::SendingTime, Tag::TargetCompId>(_msg);

        if (reject.first) {
            return reject;
        } else if (_msg.at(Tag::BeginString) != "FIX.4.2") {
            reject.second.set371_refTagId(Tag::BeginString);
            reject.second.set373_sessionRejectReason(Reject::ValueOORange);
            reject.second.set58_text("Fix protocl version not supported");
        } else if (!utils::is_numeric(_msg.at(Tag::BodyLength))) {                  // Need more test on Body Length: correspond?
            reject.second.set371_refTagId(Tag::BodyLength);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Body length isn't a number");
        } else if (!utils::is_numeric(_msg.at(Tag::MsqSeqNum))) {                   // Need more test on Msg sequence number: correspond?
            reject.second.set371_refTagId(Tag::MsqSeqNum);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Sequence number isn't a number");
        } else if (_msg.at(Tag::MsgType).size() != 1) {
            reject.second.set371_refTagId(Tag::MsgType);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Wrong message type format");
        } else if (!utils::is_numeric(_msg.at(Tag::SenderCompId))) {                // Need more test on Sender comp Id: correspond?
            reject.second.set371_refTagId(Tag::SenderCompId);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Sender Id should be numerical");
        } else if (!utils::is_numeric(_msg.at(Tag::TargetCompId))) {                // Need more test on Target comp Id: correspond?
            reject.second.set371_refTagId(Tag::TargetCompId);
            reject.second.set373_sessionRejectReason(Reject::IncorrectFormat);
            reject.second.set58_text("Target Id should be numerical");
        } else {
            reject.first = false;
        }
        return reject;
    }

    void Header::set9_bodyLength(const std::string &_val)
    {
        BodyLength = _val;
    }

    void Header::set9_bodyLength(const size_t &_val)
    {
        BodyLength = std::to_string(_val);
    }

    void Header::set35_MsgType(const std::string &_val)
    {
        MsgType = _val;
    }

    void Header::set34_msgSeqNum(const std::string &_val)
    {
        MsgSeqNum = _val;
    }

    void Header::set34_msgSeqNum(const size_t &_val)
    {
        MsgSeqNum = std::to_string(_val);
    }

    void Header::set49_SenderCompId(const std::string &_val)
    {
        SenderCompId = _val;
    }

    void Header::set56_TargetCompId(const std::string &_val)
    {
        TargetCompId = _val;
    }

    Header::operator std::string() const
    {
        return "8=" + BeginString + (char)FIX_DELIMITER +
               "9=" + BodyLength + (char)FIX_DELIMITER +
               "35=" + MsgType + (char)FIX_DELIMITER +
               "49=" + SenderCompId + (char)FIX_DELIMITER +
               "56=" + TargetCompId + (char)FIX_DELIMITER +
               "34=" + MsgSeqNum + (char)FIX_DELIMITER +
               "52=" + SendingTime + (char)FIX_DELIMITER;
    }

    void Header::setSendingTime()
    {
        time_t currentTime = time(nullptr);
        struct tm *timeInfo = localtime(&currentTime);

        // Get the microseconds since the last second
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        int microseconds = tv.tv_usec;

        // Format the current time as a string
        char dateTimeBuffer[80];
        strftime(dateTimeBuffer, sizeof(dateTimeBuffer), "%Y%m%d-%H:%M:%S", timeInfo);

        // Convert the formatted time string to a C++ string
        std::string formattedTime = dateTimeBuffer;

        // Prepend two zeros to the microsecond value
        std::string microsecondsString = std::to_string(microseconds);
        if (microsecondsString.length() < 3)
            microsecondsString = "00" + microsecondsString;

        // Combine the formatted time string with the microsecond value
        formattedTime = formattedTime + "." + microsecondsString.substr(0, 3);
        // std::string formattedTime(timeBuffer);
        SendingTime = formattedTime;
    }

    void Header::updateMsgSeqNum()
    {
        MsgSeqNum = std::to_string(std::stoi(MsgSeqNum) + 1);
    }

    std::string Header::getPartialHeader() const
    {
        return "35=" + MsgType + (char)FIX_DELIMITER +
               "49=" + SenderCompId + (char)FIX_DELIMITER +
               "56=" + TargetCompId + (char)FIX_DELIMITER +
               "34=" + MsgSeqNum + (char)FIX_DELIMITER +
               "52=" + SendingTime + (char)FIX_DELIMITER;
    }
}