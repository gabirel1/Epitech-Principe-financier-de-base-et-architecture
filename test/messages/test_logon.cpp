#include <gtest/gtest.h>
#include "Common/Message/Logon.hpp"
#include "Common/Message/Utils.hpp"

class TestLogon : public ::testing::Test
{
protected:
    void SetUp()
    {
        m_logon.set49_EncryptMedthod("0");
        m_logon.set98_EncryptMethod("0");
        m_logon.set108_HeartBtInt("30");
        m_logon.header.setSeqNum(1);

        std::string formatedLogon = m_logon.to_string();
        std::string sendingTime = formatedLogon.substr(formatedLogon.find("52=") + 3, 21);

        std::cout << "SendingTime: '" << sendingTime << "'" << std::endl;

        m_sendingTime = sendingTime;
        m_formatedLogon = formatedLogon;
    }

    fix::Logon m_logon;
    std::string m_sendingTime;
    std::string m_formatedLogon;
};

TEST_F(TestLogon, TestLogonToString)
{
    std::string expected = "8=FIX.4.2" + std::string(1, FIX_DELIMITER) +
                           "9=72" + std::string(1, FIX_DELIMITER) +
                           "35=A" + std::string(1, FIX_DELIMITER) +
                           "49=CLIENT" + std::string(1, FIX_DELIMITER) +
                           "56=TARGET" + std::string(1, FIX_DELIMITER) +
                           "34=1" + std::string(1, FIX_DELIMITER) +
                           "52=" + m_sendingTime + std::string(1, FIX_DELIMITER) +
                           "49=0" + std::string(1, FIX_DELIMITER) +
                           "98=0" + std::string(1, FIX_DELIMITER) +
                           "108=30" + std::string(1, FIX_DELIMITER);
    expected += "10=" + Utils::getChecksum(expected) + std::string(1, FIX_DELIMITER);

    std::cout << "Expected: '" << expected << "'" << std::endl;
    std::cout << "Actual: '" << m_formatedLogon << "'" << std::endl;
    EXPECT_EQ(m_formatedLogon, expected);

}