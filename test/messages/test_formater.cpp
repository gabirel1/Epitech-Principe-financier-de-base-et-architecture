#include "Common/Message/Serializer.hpp"
#include "Common/Message/Logon.hpp"
#include <gtest/gtest.h>

class SerializerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_serializer = fix::Serializer();
        m_logon = fix::Logon();

        m_logon.set98_EncryptMethod("0");
        m_logon.set108_HeartBtInt("30");
        m_logon.header.setSeqNum(1);

        std::string formatedLogon = m_logon.to_string();
        std::string sendingTime = formatedLogon.substr(formatedLogon.find("52=") + 3, 21);

        m_sendingTime = sendingTime;
        m_message = formatedLogon;
    }

    std::string m_message;
    std::string m_sendingTime;
    fix::Logon m_logon;
    fix::Serializer m_serializer;
    fix::Serializer::AnonMessage m_anonMessage;
};

TEST_F(SerializerTest, TestSerialize)
{
    fix::Serializer::Error err = m_serializer.run(m_message, m_anonMessage);

    ASSERT_EQ(err, fix::Serializer::Error::None);
    ASSERT_EQ(m_anonMessage.contains("35"), true);
    ASSERT_EQ(m_anonMessage.at("35")[0], 'A');
    ASSERT_EQ(m_anonMessage.at("49"), "CLIENT");
    ASSERT_EQ(m_anonMessage.at("56"), "TARGET");
    ASSERT_EQ(m_anonMessage.at("98"), "0");
    ASSERT_EQ(m_anonMessage.at("108"), "30");
    ASSERT_EQ(m_anonMessage.at("34"), "1");
    ASSERT_EQ(m_anonMessage.at("52"), m_sendingTime);
}

TEST_F(SerializerTest, TestSerializeInvalidKey)
{
    std::string tmp = m_message;
    tmp[14] = (char)FIX_DELIMITER;
    tmp[30] = '=';

    fix::Serializer::Error err = m_serializer.run(tmp, m_anonMessage);
    ASSERT_EQ(err == fix::Serializer::Error::InvalidKey, true);
}

TEST_F(SerializerTest, TestSerializeInvalidEnd)
{
    std::string message = m_message.substr(0, m_message.size() - 10);

    fix::Serializer::Error err = m_serializer.run(message, m_anonMessage);

    ASSERT_EQ(err == fix::Serializer::Error::InvalidEnd, true);
}

TEST_F(SerializerTest, TestSerializeNoEqual)
{
    std::string message = "8=FIX.4.2" + std::string(1, FIX_DELIMITER) +
                          "9-72" + std::string(1, FIX_DELIMITER);

    fix::Serializer::Error err = m_serializer.run(message, m_anonMessage);

    ASSERT_EQ(err == fix::Serializer::Error::NoEqual, true);
}