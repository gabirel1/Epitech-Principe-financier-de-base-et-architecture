#include "Message/Fix.hpp"

namespace fix
{
    size_t Message::sum() const
    {
        return 0;
    }

    Message::operator std::string () const
    {
        std::string str = header;

        for (const auto &[_key, _val] : m_params)
            str += _key + "=" + _val + "^";
        str += "10" + std::to_string(sum()) + "^";
        return str;
    }
}