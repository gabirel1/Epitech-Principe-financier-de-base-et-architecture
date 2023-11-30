#include <iostream>

#include "Core/Logger.hpp"

template<class ...Ts>
void Logger::Log(Ts &&..._args)
{
    LogImpl(m_file, std::forward<Ts>(_args)...);
    LogImpl(std::cout, std::forward<Ts>(_args)...);
}

template <class... Ts>
void Logger::LogImpl(std::ostream &stream, Ts &&..._args) {
    (stream << ... << std::forward<Ts>(_args)) << '\n';
}