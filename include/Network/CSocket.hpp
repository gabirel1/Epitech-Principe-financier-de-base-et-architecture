#pragma once

#include <sys/socket.h>
#include <sys/types.h>

namespace net
{
    namespace c
    {
        class Socket
        {
            public:
                Socket(int _dom, int _type, int _proto);
                ~Socket();

                [[nodiscard]] static int create(int _dom, int _type, int _proto);

                static void bind(int _fd, struct sockaddr *_addr);

                static void listen(int _fd, int _max);

                [[nodiscard]] static bool connect(int _fd, struct sockaddr *_addrs);
                [[nodiscard]] static int accept(int _fd);

                [[nodiscard]] static size_t send(int _fd, const uint8_t *_data, size_t _size);
                [[nodiscard]] static const uint8_t *receive(int _fd, size_t _size, int &_error);

                static void blocking(int _fd, bool _block);
                [[nodiscard]] static bool blocking(int _fd) const;

                [[nodiscard]] static bool close(int _fd);

            protected:
                friend class ::net::Selector;

                void create();

                void bind(struct sockaddr *_addr);

                void listen(int _max);

                [[nodiscard]] bool connect(uint32_t _ip, uint32_t _port, );
                [[nodiscard]] int accept();

                [[nodiscard]] size_t send(const uint8_t *_data, size_t _size);
                [[nodiscard]] const uint8_t *receive(size_t _size, int &_error);

                void blocking(bool _block);
                [[nodiscard]] bool isBlocking() const;

                bool close();

                void raw(int _fd);
                [[nodiscard]] int raw() const;

            private:
                int m_dom;
                int m_type;
                int m_proto;

                int m_fd = 0;
        };
    }
}