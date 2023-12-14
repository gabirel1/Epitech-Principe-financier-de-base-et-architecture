#include "Message/HeartBeat.hpp"

namespace fix
{
    HeartBeat::HeartBeat()
    {
        header.setMsgType(m_msgType);
    }

    HeartBeat::~HeartBeat() {}

    void HeartBeat::set112_TestReqID(const std::string &_val)
    {
        m_params.emplace({ "112", _val });
    }
}