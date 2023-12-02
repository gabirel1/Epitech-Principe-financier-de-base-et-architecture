#include <iostream>

#include "Core/Logger.hpp"

void Logger::Init(const std::string &_path)
{
    m_file = std::ofstream(_path, std::ofstream::out);
}

