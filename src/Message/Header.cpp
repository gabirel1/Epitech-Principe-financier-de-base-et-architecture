#include "Message/Header.hpp"

namespace fix
{
    Header::operator std::string() const
    {
        return "8=" + BeginString + "^" \
                "9=" + BodyLength + "^" \
                "35=" + MsgType + "^" \
                "49=" + SendingTime + "^" \
                "56=" + TargetCompId + "^" \
                "34=" + MsgSeqNum + "^" \
                "56=" + SendingTime + "^";
    }
}