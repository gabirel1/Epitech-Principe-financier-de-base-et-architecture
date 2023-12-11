#include "Core/Core.hpp"
#include "Core/Logger.hpp"
#include "Message/Logon.hpp"
#include "Message/Logout.hpp"

int main(int _ac, const char **_av)
{
    Logger::Init();
    Logger::SetThreadName(THIS_THREAD_ID, "Core");

    // fix::Logon msg;
    // msg.header.BeginString = "FIX.4.4";
    // msg.header.SenderCompId = "SENDER";
    // msg.header.TargetCompId = "TARGET";
    // msg.set108_HeartBtInt("30");
    // msg.set98_EncryptMethod("0");
    // std::cout << "\"" << (std::string)msg << "\"" << std::endl;

    // fix::Logout msg2;
    // msg2.header.BeginString = "FIX.4.4";
    // msg2.header.SenderCompId = "SENDER";
    // msg2.header.TargetCompId = "TARGET";
    // msg2.set58_Text("TEST_LOGOUT");
    // std::cout << "\"" << (std::string)msg2 << "\"" << std::endl;

    Core core{8080, 8081};

    core.start();

    return 0;
}