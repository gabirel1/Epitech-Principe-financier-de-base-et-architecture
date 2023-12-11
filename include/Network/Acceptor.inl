#include <cstring>

#include <netinet/in.h>

#include "Network/Acceptor.hpp"

namespace net
{
    template<IsSocket T>
    int Acceptor<T>::listen(uint32_t _port)
    {
        struct sockaddr_in addr;

        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(_port);

        create();
        bind((struct sockaddr *)&addr);
        c::Socket::listen(MAX_SOCKET);
    }

    template<IsSocket T>
    Acceptor<T>::Client Acceptor<T>::accept()
    {
        int fd = c::Socket::accept();
        Client socket = nullptr;

        if (fd == 0)
            return nullptr;
        socket = std::make_shared<T>();
        socket->raw(fd);
        return socket;
    }
}