#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include <functional>

#include "../../include/network/Connection.hpp"
#include "../../include/network/ConnectionManager.hpp"
#include "../../include/core/PokerServer.hpp"
#include "../../include/players/PlayerManager.hpp"

using namespace poker;

namespace {
    struct ConnectedSockets {
        boost::asio::ip::tcp::socket server;
        boost::asio::ip::tcp::socket client;
    };

    ConnectedSockets makeConnectedSockets(boost::asio::io_context& io) {
        using boost::asio::ip::tcp;
        tcp::endpoint listenEndpoint(boost::asio::ip::address_v4::loopback(), 0);
        tcp::acceptor acceptor(io, listenEndpoint);
        const auto port = acceptor.local_endpoint().port();
        tcp::endpoint connectEndpoint(boost::asio::ip::address_v4::loopback(), port);

        tcp::socket client(io);
        client.connect(connectEndpoint);

        tcp::socket server(io);
        acceptor.accept(server);

        return {std::move(server), std::move(client)};
    }

    bool waitUntil(boost::asio::io_context& io, const std::function<bool()>& condition,
                   std::chrono::milliseconds timeout = std::chrono::milliseconds(500)) {
        const auto deadline = std::chrono::steady_clock::now() + timeout;
        while (std::chrono::steady_clock::now() < deadline) {
            io.poll();
            if (condition()) {
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        io.poll();
        return condition();
    }

    void writeRaw(boost::asio::ip::tcp::socket& socket, const std::string& data) {
        boost::asio::write(socket, boost::asio::buffer(data));
    }
}

TEST(ConnectionTest, SpoofedPlayerIdIsOverriddenWithAuthenticatedPlayerId) {
    PokerServer server(0);
    ConnectionManager manager;
    auto& io = server.getIoContext();

    auto sockets = makeConnectedSockets(io);
    auto connection = std::make_shared<Connection>(std::move(sockets.server), server, manager);
    connection->setPlayerId(5);
    connection->start();

    for (int i = 0; i < 5; ++i) {
        server.getPlayerManager().createPlayer("p" + std::to_string(i));
    }
    auto player5 = server.getPlayerManager().getPlayer(5);
    ASSERT_NE(player5, nullptr);
    EXPECT_EQ(player5->getTableId(), -1);

    writeRaw(sockets.client, R"({"type":"join","playerId":10,"payload":""})" "\n");

    ASSERT_TRUE(waitUntil(io, [&] { return player5->getTableId() != -1; }));
}

TEST(ConnectionTest, ReadUntilProcessesTwoMessagesInSingleTcpChunk) {
    PokerServer server(0);
    ConnectionManager manager;
    auto& io = server.getIoContext();

    auto sockets = makeConnectedSockets(io);
    auto connection = std::make_shared<Connection>(std::move(sockets.server), server, manager);
    connection->start();

    writeRaw(sockets.client,
             R"({"type":"login","payload":"alice"})" "\n"
             R"({"type":"login","payload":"bob"})" "\n");

    ASSERT_TRUE(waitUntil(io, [&] { return server.getPlayerManager().getPlayer(2) != nullptr; }));
}

TEST(ConnectionTest, PartialPacketIsNotParsedUntilNewlineArrives) {
    PokerServer server(0);
    ConnectionManager manager;
    auto& io = server.getIoContext();

    auto sockets = makeConnectedSockets(io);
    auto connection = std::make_shared<Connection>(std::move(sockets.server), server, manager);
    connection->start();

    writeRaw(sockets.client, R"({"type":"login","payload":"half)");
    io.poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    io.poll();
    EXPECT_EQ(server.getPlayerManager().getPlayer(1), nullptr);

    writeRaw(sockets.client, R"(done"})" "\n");
    ASSERT_TRUE(waitUntil(io, [&] { return server.getPlayerManager().getPlayer(1) != nullptr; }));
}