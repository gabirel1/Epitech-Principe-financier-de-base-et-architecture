#include "Message/Header.hpp"
#include <ctime>
#include <sys/time.h>
#include <iomanip>
#include <iostream>
#include "Message/Fix.hpp"

namespace fix
{
    Header::Header() {}

    Header::Header(fix::Header &_header): BeginString(_header.BeginString),
                                          BodyLength(_header.BodyLength),
                                          MsgType(_header.MsgType),
                                          SenderCompId(_header.SenderCompId),
                                          TargetCompId(_header.TargetCompId),
                                          MsgSeqNum(_header.MsgSeqNum),
                                          SendingTime(_header.SendingTime)
    {}

    Header::~Header() {}

    Header::operator std::string() const
    {
        return "8=" + BeginString + (char)FIX_DELIMITER +
               "9=" +
               BodyLength + (char)FIX_DELIMITER +
               "35=" +
               MsgType + (char)FIX_DELIMITER +
               "49=" +
               SenderCompId + (char)FIX_DELIMITER +
               "56=" +
               TargetCompId + (char)FIX_DELIMITER +
               "34=" +
               MsgSeqNum + (char)FIX_DELIMITER +
               "52=" +
               SendingTime + (char)FIX_DELIMITER;
    }

    void Header::setBodyLength(const std::size_t &_len)
    {
        BodyLength = std::to_string(_len);
    }

    void Header::setMsgType(const std::string &_val)
    {
        MsgType = _val;
    }

    void Header::setSeqNum(const std::size_t &_val)
    {
        MsgSeqNum = std::to_string(_val);
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