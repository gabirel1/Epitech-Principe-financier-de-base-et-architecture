#include "Common/Message/Logout.hpp"
#include "Common/Message/Reject.hpp"

namespace fix
{
    Logout::Logout()
    {
        header.set35_MsgType(m_msgType);
    }

    std::pair<bool, Reject> Logout::Verify(Serializer::AnonMessage &_msg)
    {
        std::ignore = _msg;

        return { false, Reject{} };
    }
}