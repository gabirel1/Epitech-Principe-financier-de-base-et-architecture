#include "Network/Acceptor.hpp"

namespace net
{
    template<IsSocket T>
    int Acceptor<T>::listen(uint32_t _port)
    {
        struct sockaddr_in serv_addr;

        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(_port);

        create();
        bind(&addr);
        listen();
    }

    template<IsSocket T>
    Acceptor<T>::Client Acceptor<T>::accept()
    {
        int fd = accept();
        Client socket = nullptr;

        if (fd == 0)
            return nullptr;
        socket = std::make_shared<T>();
        socket->raw(fd);
        return socket;
    }
}