#include <ctime>
#include <sys/time.h>
#include <iomanip>
#include <iostream>
#include "Common/Message/Header.hpp"
#include "Common/Message/Fix.hpp"

namespace fix
{
    void Header::set49_SenderCompId(const std::string &_val)
    {
        SenderCompId = _val;
    }

    void Header::set56_TargetCompId(const std::string &_val)
    {
        TargetCompId = _val;
    }

    void Header::setBodyLength(const std::string &_val)
    {
        BodyLength = _val;
    }

    void Header::setBodyLength(const size_t &_val)
    {
        BodyLength = std::to_string(_val);
    }

    void Header::setMsgType(const std::string &_val)
    {
        MsgType = _val;
    }

    void Header::setSeqNum(const std::string &_val)
    {
        MsgSeqNum = _val;
    }

    void Header::setSeqNum(const size_t &_val)
    {
        MsgSeqNum = std::to_string(_val);
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