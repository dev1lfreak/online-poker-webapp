#include "../../include/core/PokerServer.hpp"
#include "../../include/protocol/MessageParser.hpp"
#include <iostream>

namespace poker {
    PokerServer::PokerServer()
        : io(),
          pool(10),
          tableManager(io),
          router(*this) {
    }

    void PokerServer::start() {
        std::cout << "Poker server started" << std::endl;

        for (int i = 0; i < 2; i++) {
            tableManager.createTable();
        }

        auto p1 = playerManager.createPlayer("Alice");
        auto p2 = playerManager.createPlayer("Bob");
        auto p3 = playerManager.createPlayer("Charlie");
        auto p4 = playerManager.createPlayer("Dave");
        auto p5 = playerManager.createPlayer("Mari");
        auto p6 = playerManager.createPlayer("Peter");

        auto p7 = playerManager.createPlayer("John");
        auto p8 = playerManager.createPlayer("Laura");
        auto p9 = playerManager.createPlayer("Jack");
        auto p10 = playerManager.createPlayer("Julia");
        auto p11 = playerManager.createPlayer("Rick");
        auto p12 = playerManager.createPlayer("Morty");

        auto table1 = tableManager.getTable(1);

        table1->addPlayer(p1);
        table1->addPlayer(p2);
        table1->addPlayer(p3);
        table1->addPlayer(p4);
        table1->addPlayer(p5);
        table1->addPlayer(p6);

        table1->startGame();

        auto table2 = tableManager.getTable(2);
        table2->addPlayer(p7);
        table2->addPlayer(p8);
        table2->addPlayer(p9);
        table2->addPlayer(p10);
        table2->addPlayer(p11);
        table2->addPlayer(p12);

        table2->startGame();

        for (std::size_t i = 0; i < 4; ++i) {
            boost::asio::post(pool, [this]() {
                io.run();
            });
        }

        pool.join();
    }

    PlayerManager &PokerServer::getPlayerManager() {
        return playerManager;
    }

    TableManager &PokerServer::getTableManager() {
        return tableManager;
    }

    ChatManager &PokerServer::getChatManager() {
        return chatManager;
    }

    ConnectionManager &PokerServer::getConnectionManager() {
        return connectionManager;
    }

    boost::asio::io_context &PokerServer::getIoContext() {
        return io;
    }

    void PokerServer::handleRawMessage(const std::string &data) {
        auto msg = MessageParser::parse(data);
        router.route(msg);
    }
}
