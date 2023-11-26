#pragma once

#include <unordered_map>
#include <string>

#include "Message/Header.hpp"
#include "InsertMap.hpp"

namespace fix
{
    class Message
    {
        public:
            Message() = default;
            virtual ~Message() = default;

            Header header;

            operator std::string () const;

            [[nodiscard]] size_t sum() const;   // 10: Sum

        protected:
            friend class Serializer;

            InsertMap<std::string, std::string> m_params{};
    };
}