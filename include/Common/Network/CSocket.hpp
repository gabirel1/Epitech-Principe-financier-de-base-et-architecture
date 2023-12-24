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

                [[nodiscard]] static int accept(int _fd);

                [[nodiscard]] static size_t send(int _fd, const uint8_t *_data, size_t _size);
                [[nodiscard]] static const uint8_t *receive(int _fd, size_t _size, int &_error);

                [[nodiscard]] static bool blocking(int _fd, bool _block);
                [[nodiscard]] static bool blocking(int _fd);

                [[nodiscard]] static bool close(int _fd);

                [[nodiscard]] bool is_open();
                [[nodiscard]] static bool is_open(int _fd);


            protected:
                template<IsSocket T>
                friend class ::net::Selector;
                template<IsSocket T>
                friend class ::net::Acceptor;

                /// @brief Create a new socket using m_dom, m_type and m_proto and set it to m_fd.
                /// @return True if the socket is succefully created otherwise false.
                [[nodiscard]] bool c_create();

                /// @brief Bind m_fd to the address from _addr.
                /// @param _addr C formated addres to bind to.
                /// @return True if the socket is succefully bind otherwise false.
                [[nodiscard]] bool c_bind(struct sockaddr *_addr);

                /// @brief Set the socket to listening/
                /// @param _max The maximum handled socket at once.
                /// @return True if the initialisation of listen is succefully otherwise false.
                [[nodiscard]] bool c_listen(int _max);

                /// @brief Connect the socket to an endpoint.
                /// @param _ip Ip of the endpoint.
                /// @param _port Port of the endpoint.
                /// @return True if the connection is succesfull otherwise false.
                [[nodiscard]] bool c_connect(const char *_ip, uint32_t _port);

                /// @brief Accept an incoming connection.
                /// @return The file description of the incoming connection or -1 if it's fail.
                [[nodiscard]] int c_accept();

                [[nodiscard]] size_t c_send(const std::string &_data);
                [[nodiscard]] size_t c_send(const uint8_t *_data, size_t _size);
                [[nodiscard]] const uint8_t *c_receive(size_t _size, int &_error);

                [[nodiscard]] bool c_blocking(bool _block);
                [[nodiscard]] bool c_blocking() const;

                [[nodiscard]] bool c_close();

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