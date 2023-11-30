#include "Core/Logger.hpp"

int main(int _ac, const char **_av)
{
    Logger::Init();

    Logger::Log("this is a test", 315, "aaaa", 34.5f);
    return 0;
}