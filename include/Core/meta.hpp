#pragma once

#include <concepts>
#include <type_traits>

#include "Core/Order.hpp"

#define THIS_THREAD_ID std::this_thread::get_id()

namespace net
{
    class Socket;
}

template<class T>
concept IsSocket = std::is_base_of_v<net::Socket, T>;

template <typename T>
concept IsBook = std::is_same_v<typename T::key_type, Price> &&
    std::is_same_v<typename T::mapped_type, OrderList> &&
    requires(T &_map, Price _price) {
        { _map.contains(_price) } -> std::same_as<bool>;
        { _map.size() } -> std::same_as<size_t>;
        { _map.begin() } -> std::same_as<typename T::iterator>;
        { _map.end() } -> std::same_as<typename T::iterator>;
    };