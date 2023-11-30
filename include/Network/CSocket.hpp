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

                [[nodiscard]] static bool connect(int _fd, uint32_t _ip, uint32_t _port, );

                [[nodiscard]] static size_t send(int _fd, const uint8_t *_data, size_t _size);
                [[nodiscard]] static const uint8_t *receive(int _fd, size_t _size, int &_error);

                static void blocking(int _fd, bool _block);
                [[nodiscard]] static bool isBlocking(int _fd) const;

                [[nodiscard]] static bool close(int _fd);


            protected:
                void create();

                [[nodiscard]] bool connect(uint32_t _ip, uint32_t _port, );

                [[nodiscard]] size_t send(const uint8_t *_data, size_t _size);
                [[nodiscard]] const uint8_t *receive(size_t _size, int &_error);

                void blocking(bool _block);
                [[nodiscard]] bool isBlocking() const;

                bool close();

                [[nodiscard]] int raw() const;

            private:
                int m_dom;
                int m_type;
                int m_proto;

                int m_fd = 0;
        };
    }
}