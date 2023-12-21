#include "Client/Core/Core.hpp"

int main(int _ac, char **_av)
{
    Core core("127.0.0.1", 8080, 8081);

    core.start();
    return 0;
}