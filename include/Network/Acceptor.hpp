#pragma once

namespace net
{
    class Acceptor
    {
        public:
            Acceptor();
            ~Acceptor();

            std::shared_ptr<Socket> accept();

            
    };
}