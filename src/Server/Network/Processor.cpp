#include "Common/Core/Logger.hpp"
#include "Server/Network/Processor.hpp"

#include "Common/Message/Serializer.hpp"
#include "Common/Message/Reject.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

namespace net
{
    namespace tcp
    {
        bool processor(ClientSocket &_socket, NetToAction &_serial, RawOutput &_error)
        {
            int error = 0;
            fix::Serializer::AnonMessage msg;
            fix::Reject reject;

            if (!_socket.getSocket()) {
                Logger::Log("[Processor] Client disconnected: "); // todo log
                return true;
            }
            std::string data(_socket.getSocket()->receive(MAX_RECV_SIZE, error));

            Logger::Log("[Processor] Received from the client: ", _socket.User, ", data:", data); // todo log
            if (error == 0) {
                Logger::Log("[Processor] Error: no data receive from the client: "); // todo log
                return true;
            }
            if (fix::Serializer::run(data, msg) != fix::Serializer::Error::None) {
                Logger::Log("[Processor] Error: will parsing the client message: "); // todo log
                // build reject
                _error.push(ErrorMsg(_socket, reject));
                return false;
            }
            Logger::Log("[Processor] Porcessing request from the client: "); // todo log
            _socket.newRequest();
            _serial.push(NetOut(_socket, msg));
            return false;
        }
    }

    namespace udp
    {
        bool processor(std::shared_ptr<net::udp::Socket> _socket)
        {
            std::ignore = _socket;

            return true;
        }
    }
}