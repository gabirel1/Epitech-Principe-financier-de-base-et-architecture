#pragma once

#include "Common/Core/Order.hpp"
#include "Client/History.hpp"

struct Context
{
    bool Loggin = false;
    UserId User;
    uint32_t HeartBit = 0;
    uint32_t SeqNum = 1;
    History userInfos;

    void reset();
};

std::ostream &operator<<(std::ostream &_os, const Context &_ctx);