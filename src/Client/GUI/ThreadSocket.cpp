#include "Client/GUI/ThreadSocket.hpp"
#include <iostream>

ThreadSocket::ThreadSocket()
{
    m_running = true;
}

ThreadSocket::~ThreadSocket()
{
}

void ThreadSocket::run()
{
    int error = 0;

    while (m_running) {
        // sleep(1);
        // std::cout << "Test running" << std::endl;
    }
}

void ThreadSocket::stop()
{
    m_running = false;
}