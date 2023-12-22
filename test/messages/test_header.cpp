#include "Common/Message/Header.hpp"
#include "Common/Message/Fix.hpp"

#include <gtest/gtest.h>

class HeaderTest : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            m_header = fix::Header();
            m_header.setBodyLength(123);
            m_header.setMsgType("A");
            m_header.setSeqNum(32);
        }

    fix::Header m_header;
};

TEST_F(HeaderTest, TestHeaderToString)
{
    std::string expected = "8=FIX.4.2" + std::string(1, FIX_DELIMITER) +
                           "9=123" + std::string(1, FIX_DELIMITER) +
                           "35=A" + std::string(1, FIX_DELIMITER) +
                           "49=CLIENT" + std::string(1, FIX_DELIMITER) +
                            "56=TARGET" + std::string(1, FIX_DELIMITER) +
                            "34=32" + std::string(1, FIX_DELIMITER) +
                            "52=" + std::string(1, FIX_DELIMITER);

    std::string actual = m_header;

    std::cout << "Expected: '" << expected << "'" << std::endl;
    std::cout << "Actual: '" << actual << "'" << std::endl;

    EXPECT_EQ(actual, expected);
}