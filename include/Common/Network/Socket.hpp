#pragma once

#include <string>
#include <type_traits>

#include <netinet/in.h>

#include "Common/Network/CSocket.hpp"
#include "Common/Network/Ip.hpp"
#include "Common/Thread/Queue.hpp"

namespace net
{
    class Socket : public c::Socket
    {
        public:
            using Output = ts::Queue<const uint8_t *>;

            ~Socket() = default;

            bool connect(const Ip &_ip, uint32_t _port);
            bool connect(uint32_t _ip, uint32_t _port);

            void blocking(bool _block);
            [[nodiscard]] bool blocking() const;

            size_t send(const std::string &_data);
            size_t send(const uint8_t *_data, size_t _size);

            [[nodiscard]] std::string receive(size_t _size, int &_error);

            [[nodiscard]] uint32_t getPort() const;

            bool close();

        protected:
            Socket(int _type);

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

                [[nodiscard]] bool broadcasting() const;
                [[nodiscard]] bool broadcastOn(uint32_t _port);
                [[nodiscard]] bool broadcast(const uint8_t *_str, size_t _size);

            private:
                bool m_broadcast = false;

                struct sockaddr_in m_broad_addr;
        };
    }
}