#pragma once

#include "../players/PlayerManager.hpp"
#include "../lobby/TableManager.hpp"
#include "../chat/ChatManager.hpp"
#include "../network/ConnectionManager.hpp"
#include "../protocol/Message.hpp"
#include "../protocol/MessageRouter.hpp"
#include <boost/asio.hpp>

namespace poker {
    class PokerServer {
    public:
        PokerServer();

        void start();

        PlayerManager &getPlayerManager();

        TableManager &getTableManager();

        ChatManager &getChatManager();

        ConnectionManager &getConnectionManager();

        boost::asio::io_context &getIoContext();

        void handleRawMessage(const std::string &data);

    private:
        boost::asio::io_context io;
        boost::asio::thread_pool pool;

        PlayerManager playerManager;
        TableManager tableManager;
        ChatManager chatManager;
        ConnectionManager connectionManager;
        MessageRouter router;
    };
}
