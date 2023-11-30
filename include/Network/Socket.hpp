#pragma once

#include <string>

#include "Network/CSocket.hpp"
#include "Network/Ip.hpp"
#include "Thread/Queue.hpp"

namespace net
{
    class Socket : public c::Socket
    {
        private:
            using Output = ts::Queue<const uint8_t *>;

            virtual ~Socket() = default;

            void connect(const Ip &_ip, uint32_t _port);
            void connect(uint32_t _ip, uint32_t _port);

            void blocking(bool _block);
            [[nodiscard]] bool blocking() const;

            size_t send(const std::string &_data);
            size_t send(const uint8_t *_data, size_t _size);

            [[nodiscard]] std::string receive(int &_error);

            bool close();

        protected:
            Socket(int _type);

            int create(int _dom);

        private:
            int m_type = 0;
    };

    namespace tcp
    {
        class Socket : public ::net::Socket
        {
            public:
                Socket();
                ~Socket() = default;
        };
    }

    namespace udp
    {
        class Socket : public ::net::Socket
        {
            public:
                Socket();
                ~Socket() = default;
        };
    }
}