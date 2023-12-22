#include <cstring>

#include <netinet/in.h>

#include "Common/Core/Logger.hpp"
#include "Common/Network/Acceptor.hpp"

namespace net
{
    template<IsSocket T>
    Acceptor<T>::Acceptor()
        : c::Socket(AF_INET, SOCK_STREAM, 0) // change for TCP or UDP
    {
        Logger::Log("[Acceptor] New acceptor");
    }


    template<IsSocket T>
    bool Acceptor<T>::listen(uint32_t _port)
    {
        struct sockaddr_in addr;
        int error = 0;

        Logger::Log("[Acceptor] Initialisation of new listner");
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(_port);

        Logger::Log("[Acceptor] Create new listner socket");
        create();
        Logger::Log("[Acceptor] Bind new listner socket");
        error = bind((struct sockaddr *)&addr);
        if (error != 0) {
            Logger::Log("[Acceptor] Bind failed: ", strerror(error));
            (void)close();
            return false;
        }
        Logger::Log("[Acceptor] Initialisation of the listner on port: ", _port);
        c::Socket::listen(MAX_SOCKET);
        if (error != 0) {
            Logger::Log("[Acceptor] Listening failed: ", strerror(error));
            (void)close();
            return false;
        }
        Logger::Log("[Acceptor] New listener on port: ", _port);
        return true;
    }

    template<IsSocket T>
    Acceptor<T>::Client Acceptor<T>::accept()
    {
        int fd = c::Socket::accept();
        Client socket = nullptr;

        if (fd == -1)
            return nullptr;
        socket = std::make_shared<T>();
        socket->raw(fd);
        return socket;
    }
}