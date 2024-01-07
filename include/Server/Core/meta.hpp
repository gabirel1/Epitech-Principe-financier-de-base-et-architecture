#pragma once

#include <concepts>
#include <type_traits>

#include "Common/Core/meta.hpp"
#include "Server/Core/Order.hpp"

template <typename T>
concept IsBook = std::is_same_v<typename T::key_type, Price> &&
    std::is_same_v<typename T::mapped_type, OrderList> &&
    std::is_same_v<typename T::value_type, std::pair<const typename T::key_type, typename T::mapped_type>> &&
    requires(T &_map, Price _price) {
        { _map.size() } -> std::same_as<typename T::size_type>;
        { _map.begin() } -> std::same_as<typename T::iterator>;
        { _map.end() } -> std::same_as<typename T::iterator>;
        { _map.erase(_price) } -> std::same_as<typename T::size_type>;
        { _map.contains(_price) } -> std::same_as<bool>;
        { _map.at(_price) } -> std::same_as<typename T::mapped_type &>;
    };
