#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/asio.hpp>

#define private public
#include "../../include/network/ConnectionManager.hpp"
#undef private

#include "../../include/network/Connection.hpp"
#include "../../include/core/PokerServer.hpp"

using namespace poker;
using ::testing::_;

namespace {
    class MockConnection : public Connection {
    public:
        MockConnection(boost::asio::ip::tcp::socket socket, PokerServer& server, ConnectionManager& manager)
            : Connection(std::move(socket), server, manager) {}

        MOCK_METHOD(void, send, (const Message& msg), (override));
    };
}

TEST(ConnectionManagerTest, AddAndRemoveChangeUnauthenticatedSize) {
    boost::asio::io_context io;
    PokerServer server(0);
    ConnectionManager manager;

    auto conn = std::make_shared<MockConnection>(boost::asio::ip::tcp::socket(io), server, manager);

    EXPECT_EQ(manager.unauthenticated_.size(), 0u);
    manager.add(conn);
    EXPECT_EQ(manager.unauthenticated_.size(), 1u);

    manager.remove(conn);
    EXPECT_EQ(manager.unauthenticated_.size(), 0u);
}

TEST(ConnectionManagerTest, AuthenticateWithSamePlayerIdKeepsLatestConnection) {
    boost::asio::io_context io;
    PokerServer server(0);
    ConnectionManager manager;

    auto conn1 = std::make_shared<MockConnection>(boost::asio::ip::tcp::socket(io), server, manager);
    auto conn2 = std::make_shared<MockConnection>(boost::asio::ip::tcp::socket(io), server, manager);
    conn1->setPlayerId(7);
    conn2->setPlayerId(7);

    manager.add(conn1);
    manager.add(conn2);
    manager.authenticate(conn1, 7);
    manager.authenticate(conn2, 7);

    Message msg{MessageType::Chat, 7, -1, 0, "hello"};
    EXPECT_CALL(*conn1, send(_)).Times(0);
    EXPECT_CALL(*conn2, send(_)).Times(1);
    manager.sendTo(7, msg);
}

TEST(ConnectionManagerTest, BroadcastAllSendsOnlyToAuthenticatedConnections) {
    boost::asio::io_context io;
    PokerServer server(0);
    ConnectionManager manager;

    auto auth1 = std::make_shared<MockConnection>(boost::asio::ip::tcp::socket(io), server, manager);
    auto auth2 = std::make_shared<MockConnection>(boost::asio::ip::tcp::socket(io), server, manager);
    auto unauth = std::make_shared<MockConnection>(boost::asio::ip::tcp::socket(io), server, manager);

    auth1->setPlayerId(1);
    auth2->setPlayerId(2);

    manager.add(auth1);
    manager.add(auth2);
    manager.add(unauth);
    manager.authenticate(auth1, 1);
    manager.authenticate(auth2, 2);

    Message msg{MessageType::Chat, 0, -1, 0, "broadcast"};
    EXPECT_CALL(*auth1, send(_)).Times(1);
    EXPECT_CALL(*auth2, send(_)).Times(1);
    EXPECT_CALL(*unauth, send(_)).Times(0);
    manager.broadcastAll(msg);
}