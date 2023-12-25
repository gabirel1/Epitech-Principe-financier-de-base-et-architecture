#pragma once

#include <unordered_map>
#include <string>

#include "Common/Message/Header.hpp"

 // change to '^' for testing or 1 for production
#define FIX_DELIMITER 1

namespace fix
{
    class Message
    {
        public:
            Message() = default;
            virtual ~Message() = default;

            Header header;

            operator std::string();
            std::string to_string();

            static std::size_t getBodyLength(const std::string &_str);
            static std::string getChecksum(const std::string &_str);

        protected:
            friend class Serializer;

            InsertMap<std::string, std::string> m_params{};
    };
}