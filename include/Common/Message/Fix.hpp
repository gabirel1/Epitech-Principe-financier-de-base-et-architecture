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

            template<const char *T, const char *...Ts>
            static std::pair<bool, Reject> Has(Serializer::AnonMessage &_msg);
            template<const char * T>
            static std::pair<bool, Reject> Has(Serializer::AnonMessage &_msg);

        protected:
            friend class Serializer;

            InsertMap<std::string, std::string> m_params{};

        private:
            static std::size_t getBodyLength(const std::string &_str);
            static std::string getChecksum(const std::string &_str);
    };
}

#include "Common/Message/Fix.inl"