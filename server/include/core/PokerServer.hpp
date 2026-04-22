#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <vector>
#include <cstdint>

#include "../players/PlayerManager.hpp"
#include "../lobby/TableManager.hpp"
#include "../chat/ChatManager.hpp"
#include "../network/ConnectionManager.hpp"
#include "../protocol/MessageRouter.hpp"

namespace poker {
    class PokerServer {
    public:
        explicit PokerServer(uint16_t port = 8080);
        ~PokerServer();

        void start();

        void stop();

        PlayerManager& getPlayerManager() { return playerManager_; }
        TableManager& getTableManager() { return tableManager_; }
        ChatManager& getChatManager() { return chatManager_; }
        ConnectionManager& getConnectionManager() { return connectionManager_; }
        MessageRouter& getRouter() { return router_; }
        boost::asio::io_context& getIoContext() { return io_; }
        void handleDisconnect(PlayerId playerId);

    private:
        void doAccept();

        void setupSignalHandling();

        boost::asio::io_context io_;
        boost::asio::signal_set signals_;
        boost::asio::ip::tcp::acceptor acceptor_;

        PlayerManager playerManager_;
        TableManager tableManager_;
        ChatManager chatManager_;
        ConnectionManager connectionManager_;
        MessageRouter router_;

        std::vector<std::thread> threadPool_;
    };
}