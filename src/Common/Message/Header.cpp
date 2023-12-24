#include <iomanip>
#include <ctime>
#include <iostream>

#include <sys/time.h>

#include "Common/Message/Header.hpp"
#include "Common/Message/Fix.hpp"
#include "Common/Message/Reject.hpp"

namespace fix
{
    std::pair<bool, Reject> Header::Verify(Serializer::AnonMessage &_msg)
    {
        // need to verify body length, sender, target, sequence number, sending time
        std::pair<bool, Reject> reject{ true, {} };

        if (!_msg.contains("8") || !_msg.contains("9") || !_msg.contains("35")
            || !_msg.contains("49") || !_msg.contains("56") || !_msg.contains("52")) {
            if (!_msg.contains("8"))
                reject.second.set371_refTagId("8");
            else if (!_msg.contains("9"))
                reject.second.set371_refTagId("9");
            else if (!_msg.contains("35"))
                reject.second.set371_refTagId("35");
            else if (!_msg.contains("49"))
                reject.second.set371_refTagId("49");
            else if (!_msg.contains("56"))
                reject.second.set371_refTagId("56");
            else if (!_msg.contains("52"))
                reject.second.set371_refTagId("52");
            reject.second.set373_sessionRejectReason("1");
            reject.second.set58_Text("Unable to find required field");
        } else if (_msg.at("8") != "FIX.4.2") {
            reject.second.set371_refTagId("8");
            reject.second.set373_sessionRejectReason("5");
            reject.second.set58_Text("Fix protocl version not supported");
        } else if (_msg.at("35").size() != 1) {
            reject.second.set371_refTagId("35");
            reject.second.set373_sessionRejectReason("6");
            reject.second.set58_Text("Wrong message type format");
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
        {
            microsecondsString = "00" + microsecondsString;
        }

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
               "49=" +
               SenderCompId + (char)FIX_DELIMITER +
               "56=" +
               TargetCompId + (char)FIX_DELIMITER +
               "34=" +
               MsgSeqNum + (char)FIX_DELIMITER +
               "52=" +
               SendingTime + (char)FIX_DELIMITER;
    }
}