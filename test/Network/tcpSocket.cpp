#include <gtest/gtest.h>

#include <iostream>

#include "Common/Network/Socket.hpp"
#include "Common/Network/Acceptor.hpp"

#define TEST_IP_TCP "127.0.0.1"

class tcp_Socket_ConnectClose : public testing::Test
{
    protected:
        void SetUp() override
        {
            Logger::SetThreadName(THIS_THREAD_ID, "test::tcp_Socket_ConnectClose");
            acceptor.listen(0);
            port = acceptor.getPort();
        }

        void TearDown() override
        {
            (void)acceptor.close();
        }

        net::Acceptor<net::tcp::Socket> acceptor;
        uint32_t port = 0;
};

TEST_F(tcp_Socket_ConnectClose, connect_close_server) {
    net::tcp::Socket socket;

    ASSERT_TRUE(socket.connect(TEST_IP_TCP, port));

    net::Acceptor<net::tcp::Socket>::Client client = acceptor.accept();

    ASSERT_NE(client, nullptr);
    EXPECT_TRUE(client->close());
    EXPECT_FALSE(client->is_open());
    EXPECT_FALSE(socket.is_open());
}

TEST_F(tcp_Socket_ConnectClose, connect_close_client) {
    net::tcp::Socket socket;

    ASSERT_TRUE(socket.connect(TEST_IP_TCP, port));

    net::Acceptor<net::tcp::Socket>::Client client = acceptor.accept();

    ASSERT_NE(client, nullptr);

    ASSERT_TRUE(client->close());
    ASSERT_FALSE(socket.is_open());
    ASSERT_FALSE(client->is_open());
}

class tcp_Socket_SendRecv : public testing::Test
{
    protected:
        void SetUp() override
        {
            Logger::SetThreadName(THIS_THREAD_ID, "test::tcp_Socket_SendRecv");
            acceptor.listen(0);
            socket.connect(TEST_IP_TCP, acceptor.getPort());
            client = acceptor.accept();
        }

        void TearDown() override
        {
            (void)acceptor.close();
            (void)socket.close();
            client = nullptr;
        }

        net::tcp::Socket socket;
        net::Acceptor<net::tcp::Socket> acceptor;
        net::Acceptor<net::tcp::Socket>::Client client;
};

TEST_F(tcp_Socket_SendRecv, single_send)
{
    const std::string send1 = "this is a test";
    int error = 0;

    std::cout << "send" << std::endl;
    ASSERT_EQ(socket.send(send1), send1.size());

    std::cout << "receive" << std::endl;
    std::string recv = client->receive(send1.size() + 1, error);

    ASSERT_EQ(error, 0);
    ASSERT_EQ(recv, "this is a test");
}

TEST_F(tcp_Socket_SendRecv, multiple_send_single_recv)
{
    const std::string send1 = "this is ";
    const std::string send2 = "a test";
    int error = 0;

    ASSERT_EQ(socket.send(send1), send1.size());
    ASSERT_EQ(socket.send(send2), send2.size());

    std::string recv = client->receive(send1.size() + send2.size() + 1, error);

    ASSERT_EQ(error, 0);
    ASSERT_EQ(recv, send1 + send2);
}