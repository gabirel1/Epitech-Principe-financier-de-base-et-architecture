#include "Client/GUI/ThreadSocket.hpp"
#include <iostream>

std::string data_to_send = "Hello, UDP!";

ThreadSocket::ThreadSocket()
{

    m_udp.connect("127.0.0.1", 8081);
    std::cout << "ThreasSocket Constructeur => " << m_udp.getPort() << std::endl;
    m_running = true;
}

ThreadSocket::~ThreadSocket()
{
}

void ThreadSocket::run()
{
    int error = 0;
    size_t size = 50;

    while (m_running) {

        std::cout << "ThreadSocket PORT => "<< m_udp.getPort() <<std::endl;
        std::string pack = m_udp.receive(sizeof(data_to_send), error);
        std::cout << pack << std::endl; 
    }
}

void ThreadSocket::stop()
{
    m_running = false;
}