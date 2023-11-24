#pragma once

#include <unordered_map>
#include <string>

#include "Message/Header.hpp"

namespace fix
{
    class Message
    {
        public:
            Message() = default;
            virtual ~Message() = default;

            Header header;

            operator std::string () const;

            size_t sum() const;

        protected:
            std::unordered_map<std::string, std::string> m_params;
    };
}