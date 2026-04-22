#include "../../include/protocol/MessageRouter.hpp"
#include "../../include/core/PokerServer.hpp"
#include "../../include/players/PlayerManager.hpp"
#include "../../include/lobby/TableManager.hpp"
#include "../../include/chat/ChatManager.hpp"
#include <iostream>

namespace poker {
    MessageRouter::MessageRouter(PokerServer &s)
        : server(s) {
    }

    void MessageRouter::route(const Message &msg) {
        switch (msg.type) {
            case MessageType::Login: {
                break;
            }
            case MessageType::JoinTable: {
                auto &pm = server.getPlayerManager();
                auto &tm = server.getTableManager();
                auto &cm = server.getConnectionManager();
                auto player = pm.getPlayer(msg.playerId);
                if (!player) break;

                auto table = tm.findFreeTable();
                if (!table) table = tm.createTable();

                player->setTableId(table->getId());
                table->addPlayer(player);

                Message response;
                response.type = MessageType::JoinTable;
                response.playerId = player->getId();
                response.tableId = table->getId();
                response.payload = "joined";
                cm.sendTo(player->getId(), response);
                table->publishState();
                break;
            }
            case MessageType::PlayWithBots: {
                auto &pm = server.getPlayerManager();
                auto &tm = server.getTableManager();
                auto &cm = server.getConnectionManager();
                auto player = pm.getPlayer(msg.playerId);
                if (!player) break;

                auto table = tm.createTable();

                player->setTableId(table->getId());
                table->addPlayer(player);

                for (size_t i = 0; i < 5; ++i) {
                    const auto botName = "BOT_" + std::to_string(i + 1);
                    auto bot = pm.createPlayer(botName);
                    bot->setTableId(table->getId());
                    table->addPlayer(bot);
                }

                Message response;
                response.type = MessageType::PlayWithBots;
                response.playerId = player->getId();
                response.tableId = table->getId();
                response.payload = "joined_with_bots";
                cm.sendTo(player->getId(), response);
                table->publishState();
                break;
            }
            case MessageType::StartGame: {
                auto &pm = server.getPlayerManager();
                auto &tm = server.getTableManager();
                auto player = pm.getPlayer(msg.playerId);
                if (!player) break;
                int tableId = player->getTableId();
                if (tableId < 0) break;
                auto table = tm.getTable(tableId);
                if (!table) break;
                table->requestStartGame();
                break;
            }
            case MessageType::Bet:
            case MessageType::Call:
            case MessageType::Check:
            case MessageType::Fold: {
                auto &pm = server.getPlayerManager();
                auto &tm = server.getTableManager();
                auto player = pm.getPlayer(msg.playerId);
                if (!player) break;

                int tableId = player->getTableId();
                if (tableId < 0) break;

                auto table = tm.getTable(tableId);
                if (!table) break;

                table->handleMessage(msg, player);
                break;
            }
            case MessageType::LeaveTable: {
                auto &pm = server.getPlayerManager();
                auto &tm = server.getTableManager();
                auto player = pm.getPlayer(msg.playerId);
                if (!player) break;
                int tableId = player->getTableId();
                if (tableId < 0) break;
                auto table = tm.getTable(tableId);
                if (!table) break;
                table->disconnectPlayer(player->getId());
                break;
            }
            case MessageType::Disconnect: {
                std::cout << "Disconnect requested by player " << msg.playerId << std::endl;
                server.handleDisconnect(msg.playerId);
                break;
            }
            case MessageType::Chat: {
                auto &chat = server.getChatManager();
                chat.addMessage(msg.payload);
                break;
            }
            case MessageType::Action:
            case MessageType::State:
                break;
        }
    }
}
