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

class BodyLengthTest : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            m_message = "hello i am 29 characters long";
        }

    std::string m_message;
};

TEST_F(BodyLengthTest, TestBodyLength)
{
    std::size_t expected = 29;
    std::size_t actual = Utils::getBodyLength(m_message);

    std::cout << "Expected: '" << expected << "'" << std::endl;
    std::cout << "Actual: '" << actual << "'" << std::endl;

    EXPECT_EQ(actual, expected);
}