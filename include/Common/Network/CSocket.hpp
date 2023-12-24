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

                /// @brief C++ binding to the C function socket.
                /// @param _dom The familly of protocol uesd.
                /// @param _type The type of logique for communication.
                /// @param _proto 
                /// @return The file descriptor created.
                [[nodiscard]] static int create(int _dom, int _type, int _proto);

                [[nodiscard]] static int accept(int _fd);

                static size_t send(int _fd, const uint8_t *_data, size_t _size);
                [[nodiscard]] static const uint8_t *receive(int _fd, size_t _size, int &_error);

                static bool blocking(int _fd, bool _block);
                [[nodiscard]] static bool blocking(int _fd);
                [[nodiscard]] bool blocking() const;


                static bool close(int _fd);

                [[nodiscard]] uint32_t getPort() const;
                [[nodiscard]] static uint32_t getPort(int _fd);

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
                bool c_listen(int _max);

                /// @brief Connect the socket to an endpoint.
                /// @param _ip Ip of the endpoint.
                /// @param _port Port of the endpoint.
                /// @return True if the connection is succesfull otherwise false.
                [[nodiscard]] bool c_connect(const char *_ip, uint32_t _port);

                /// @brief Accept an incoming connection.
                /// @return The file description of the incoming connection or -1 if it's fail.
                [[nodiscard]] int c_accept();

                /// @brief Send an std::string to the endpoint.
                /// @param _data the string to send.
                /// @return The number of bytes send.
                size_t c_send(const std::string &_data);

                /// @brief Send raw dara to the endpoint.
                /// @param _data List of bytes to send.
                /// @param _size Size of the data to send.
                /// @return The number of bytes send.
                size_t c_send(const uint8_t *_data, size_t _size);

                /// @brief Receive data from the connected pair. If the socket is non blocking it can return a nullptr value and _error will be set accordingly.
                /// @param _size Size that you wan't to receive.
                /// @param _error Error code return by underlined C function recv.
                /// @return The list of bytes receives.
                [[nodiscard]] const uint8_t *c_receive(size_t _size, int &_error);

                /// @brief Set the socket blocking policy.
                /// @param _block True if the socket should be blocking else false.
                /// @return If the change as been succefully apply.
                bool c_blocking(bool _block);

                bool c_close();

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