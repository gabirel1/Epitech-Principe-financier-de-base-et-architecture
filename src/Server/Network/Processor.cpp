#include "Common/Core/Logger.hpp"
#include "Server/Network/Processor.hpp"

#include "Common/Message/Serializer.hpp"
#include "Common/Message/Reject.hpp"
#include "Server/Core/Pipeline/Naming.hpp"

namespace net::tcp
{
    namespace in
    {
        bool Basic::run(ClientSocket &_client, InAction &_serial, InOutNetwork &_error)
        {
            int error = 0;
            fix::Serializer::AnonMessage msg;
            fix::Reject reject;

            if (!_client.getSocket()) {
                Logger::Log("[Processor] Client disconnected: "); // todo log
                return true;
            }
            std::string data(_client.getSocket()->receive(MAX_RECV_SIZE, error));

            Logger::Log("[Processor] Received from the client: ", _client.User, ", data:", data); // todo log
            if (error == 0) {
                Logger::Log("[Processor] Error: no data receive from the client: "); // todo log
                return true;
            }
            if (fix::Serializer::run(data, msg) != fix::Serializer::Error::None) {
                Logger::Log("[Processor] Error: will parsing the client message: "); // todo log
                // build reject
                _error.append(_client, std::move(reject));
                return false;
            }
            Logger::Log("[Processor] Porcessing request from the client: "); // todo log
            _client.newRequest();
            _serial.append(_client, std::move(msg));
            return false;
        }
    }

    namespace out
    {
        namespace priv
        {
            ClientInfo getClientInfo(OutNetworkInput &_input, std::vector<ClientSocket> &_clients)
            {
                std::vector<ClientSocket>::iterator it;
                UserId userId = "";

                if (_input.Client.getSocket())
                    return getClientInfo(_input.Client.getSocket(), _clients);
                return getClientInfo(_input.Client.User, _clients);
            }

            ClientInfo getClientInfo(const std::string &_userId, std::vector<ClientSocket> &_clients)
            {
                std::vector<ClientSocket>::iterator it;
                UserId userId = "";

                it = std::find_if(_clients.begin(), _clients.end(), [_userId = _userId] (const ClientSocket &_client) {
                    return _client.User == _userId;
                });
                if (it != _clients.end())
                    userId = _userId;
                Logger::Log("[OutNetwork] (ClientInfo) Comming from an anonymous pipeline");
                return ClientInfo(it, userId);
            }

            ClientInfo getClientInfo(std::shared_ptr<net::tcp::Socket> _socket, std::vector<ClientSocket> &_clients)
            {
                std::vector<ClientSocket>::iterator it;
                UserId userId = "";

                it = std::find_if(_clients.begin(), _clients.end(), [_socket = _socket] (const ClientSocket &_client) {
                    return _client.getSocket() == _socket;
                });
                if (it != _clients.end())
                    userId = it->User;
                Logger::Log("[OutNetwork] (Reply) Comming from an Action pipeline");
                return ClientInfo(it, userId);
            }
        }

        bool Response::run(OutNetworkInput &_data, std::vector<ClientSocket> &_clients)
        {
            auto [client, userId] = priv::getClientInfo(_data, _clients);

            _data.Message.header.set49_SenderCompId(PROVIDER_NAME);
            if (client != _clients.end()) {
                _data.Message.header.set34_msgSeqNum(std::to_string((client->SeqNumber)++));
                if (userId != "")
                    _data.Message.header.set56_TargetCompId(userId);
                std::string data = _data.Message.to_string();

                if (client->getSocket()) {
                    if (client->getSocket()->send(reinterpret_cast<const uint8_t *>(data.c_str()), data.size()) == data.size())
                        Logger::Log("[Responce] Data send successfuly: ", data);
                    else
                        Logger::Log("[Responce] Error occured when sending data");
                    client->Logged = _data.Client.Logged;
                    client->User = userId;
                    client->Disconnect = _data.Client.Disconnect;
                    LogTiming(client);
                    Logger::Log("[Responce] Updated client status: "); // todo log
                    if (_data.Client.Disconnect) {
                        client->getSocket()->close();
                        Logger::Log("[Responce] Client has been disconnected: ", userId);
                    }
                } else {
                    Logger::Log("[Responce] Client not connected: ", userId);
                    _clients.erase(client);
                    return true;
                }
            } else {
                return false;
                Logger::Log("[Responce] Client not found: ", userId);
            }
            return false;
        }

        void Response::LogTiming(std::vector<ClientSocket>::iterator _it)
        {
            if (!_it->hasRequest(_it->SeqNumber - 1))
                return;
            auto start = _it->getRequest(_it->SeqNumber - 1);
            auto end = std::chrono::system_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            Logger::Log("[Responce] Send respond after: ", diff.count(), " ms");
        }

        bool Notify::run(NotifNetworkInput &_input, std::vector<ClientSocket> &_clients)
        {

            _input.Message.header.set49_SenderCompId(PROVIDER_NAME);
            for (auto &_client : _clients) {
                if (_client.isSubscribe(_input.Symbol, _input.SubType))
                    _input.Message.header.set56_TargetCompId(_client.User);
                _input.Message.header.set34_msgSeqNum(std::to_string((_client.SeqNumber)++));

                const std::string data = _input.Message.to_string();

                if (_client.getSocket()->send(reinterpret_cast<const uint8_t *>(data.c_str()), data.size()) == data.size())
                    Logger::Log("[Notify] Data send successfuly: ", data);
                else
                    Logger::Log("[Notify] Error occured when sending data");
            }
            return true;
        }
    }
}