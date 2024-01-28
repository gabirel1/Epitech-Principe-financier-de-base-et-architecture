#include "../../../include/Client/GUI/ThreadHeartBeat.hpp"
#include <iostream>

ThreadHeartBeat::ThreadHeartBeat()
{
}

ThreadHeartBeat::~ThreadHeartBeat()
{
}

void ThreadHeartBeat::run() {

    while (true) {
        emit callHeartBeat();
        sleep(30);
    }
}