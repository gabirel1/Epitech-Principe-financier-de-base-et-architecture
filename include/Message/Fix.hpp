#pragma once

#include <unordered_map>
#include <string>

#include "Message/Header.hpp"
#include "Container/InsertMap.hpp"

#define FIX_DELIMITER '^'

namespace fix
{
    class Message
    {
        public:
            Message() = default;
            virtual ~Message() = default;

            Header header;

            // operator std::string () const;
            operator std::string ();

            [[nodiscard]] std::string checkSum(const std::string &_val) const;   // 10: Sum

        protected:
            friend class Serializer;

            InsertMap<std::string, std::string> m_params{};
    };
}