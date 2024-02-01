#pragma once

#include "Common/Core/Order.hpp"

struct Context
{
    bool Loggin = false;
    UserId User;
    uint32_t HeartBit = 0;
    uint32_t SeqNum = 0;
};

std::ostream &operator<<(std::ostream &_os, const Context &_ctx);