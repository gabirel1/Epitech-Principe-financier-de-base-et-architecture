#pragma once

#include <unordered_map>
#include <string>

#include "Message/Header.hpp"
#include "Container/InsertMap.hpp"

#define FIX_DELIMITER 1 // change to '^' for testing or 1 for production

namespace fix
{
    class Message
    {
        public:
            Message() = default;
            virtual ~Message() = default;

            Header header;

            operator std::string ();

        protected:
            friend class Serializer;

            InsertMap<std::string, std::string> m_params{};
    };
}