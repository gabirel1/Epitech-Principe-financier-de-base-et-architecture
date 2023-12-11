#include "Core/Core.hpp"

int main(int _ac, const char **_av)
{
    Core core{8080, 8081};

    core.start();
    return 0;
}