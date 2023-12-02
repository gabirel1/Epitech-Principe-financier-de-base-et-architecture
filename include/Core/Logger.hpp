#pragma once

#include <concepts>
#include <fstream>
#include <string>

template<class T>
concept ToString = requires (T _val) {
    { std::to_string(_val) } -> std::convertible_to<std::string>;
};

#ifndef LOG_DFT_FILE
    #define LOG_DFT_FILE "./default.log"
#endif

class Logger
{
    public:
        static void Init(const std::string &_path = LOG_DFT_FILE);

        template<class ...Ts>
        static void Log(Ts &&..._args);

    private:
        template<class ...Ts>
        static void LogImpl(std::ostream &_os, Ts &&..._val);

        inline static std::ofstream m_file;
};

#include "Core/Logger.inl"