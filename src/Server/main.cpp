#include "Common/Core/Logger.hpp"
#include "Server/Core/Core.hpp"

int main(int _ac, const char **_av)
{
    Logger::Init();
    Logger::SetThreadName(THIS_THREAD_ID, "Core");

    Core core{8080, 8081};

    core.start();
    return 0;
}