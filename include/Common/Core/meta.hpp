#pragma once

#include <type_traits>
#include <thread>

#define THIS_THREAD_ID std::this_thread::get_id()

namespace net
{
    class Socket;
}

template<class T>
concept IsSocket = std::is_base_of_v<net::Socket, T>;
