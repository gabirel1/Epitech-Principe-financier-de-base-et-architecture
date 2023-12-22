#include <gtest/gtest.h>

#include "Common/Message/Utils.hpp"
#include "Common/Message/Fix.hpp"

class ChecksumTest : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            m_message = std::string("this is a t3st!!") + (char)FIX_DELIMITER;
            std::cout << "Message: '" << m_message << "'" << std::endl;
        }

    std::string m_message;
};


TEST_F(ChecksumTest, TestChecksum)
{
    std::string expected = "038";
    std::cout << "Message: '" << m_message << "'" << std::endl;
    std::string actual = Utils::getChecksum(m_message);

    std::cout << "Expected: '" << expected << "'" << std::endl;
    std::cout << "Actual: '" << actual << "'" << std::endl;

    EXPECT_EQ(actual, expected);
}
