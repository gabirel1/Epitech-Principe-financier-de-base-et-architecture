#pragma once

#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "Network/Ip.hpp"
#include "Thread/Queue.hpp"

namespace net
{
    class Socket
    {
        private:
            using Output = ts::Queue<const uint8_t *>;

            virtual ~Socket() = default;

            void connect(const Ip &_ip, uint32_t m_port);

            void blocking(bool _block);
            [[nodiscard]] bool isBlocking() const;

            size_t send(const std::string &_data);
            size_t send(const uint8_t *_data, size_t _size);

            [[nodiscard]] const uint8_t *receive(int &_error);

            bool close();

        protected:
            Socket(int _type);

            int create(int _dom, int _type);

            [[nodiscard]] int raw() const;

        private:
            bool m_block = true;
            int m_type = 0;

            int m_fd;
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