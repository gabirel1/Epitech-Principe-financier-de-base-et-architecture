#pragma once

#include <string>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstdint>

#include "Common/Core/meta.hpp"

namespace net
{
    template<IsSocket T>
    class Selector;

    template<IsSocket T>
    class Acceptor;

    namespace c
    {
        class Socket
        {
            public:
                Socket() = default;
                Socket(int _dom, int _type, int _proto);
                ~Socket();

                [[nodiscard]] static int create(int _dom, int _type, int _proto);

                static void bind(int _fd, struct sockaddr *_addr, size_t _size);

                static void listen(int _fd, int _max);

                [[nodiscard]] static bool connect(int _fd, struct sockaddr *_addrs, size_t _size);
                [[nodiscard]] static int accept(int _fd);

                [[nodiscard]] static size_t send(int _fd, const uint8_t *_data, size_t _size);
                [[nodiscard]] static const uint8_t *receive(int _fd, size_t _size, int &_error);

                [[nodiscard]] static bool blocking(int _fd, bool _block);
                [[nodiscard]] static bool blocking(int _fd);

                [[nodiscard]] static bool close(int _fd);

            protected:
                template<IsSocket T>
                friend class ::net::Selector;
                template<IsSocket T>
                friend class ::net::Acceptor;

                void create();

                void bind(struct sockaddr *_addr);

                void listen(int _max);

                [[nodiscard]] bool connect(const char *_ip, uint32_t _port);
                [[nodiscard]] int accept();

                [[nodiscard]] size_t send(const std::string &_data);
                [[nodiscard]] size_t send(const uint8_t *_data, size_t _size);
                [[nodiscard]] const uint8_t *receive(size_t _size, int &_error);

                [[nodiscard]] bool blocking(bool _block);
                [[nodiscard]] bool blocking() const;

                [[nodiscard]] bool close();

                void raw(int _fd);
                [[nodiscard]] int raw() const;

            private:
                int m_dom = 0;
                int m_type = 0;
                int m_proto = 0;

                int m_fd = 0;
        };
    }
}