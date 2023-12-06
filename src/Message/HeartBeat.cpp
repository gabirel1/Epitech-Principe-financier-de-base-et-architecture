#include "Message/HeartBeat.hpp"

namespace fix
{
    void HeartBeat::set112_TestReqID(const std::string &_val)
    {
        m_params.emplace({ "112", _val });
    }
}