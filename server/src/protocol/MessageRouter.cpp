#include "../../include/protocol/MessageRouter.hpp"
#include "../../include/core/PokerServer.hpp"
#include "../../include/players/PlayerManager.hpp"
#include "../../include/lobby/TableManager.hpp"
#include "../../include/chat/ChatManager.hpp"

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
                auto player = pm.getPlayer(msg.playerId);
                if (!player) break;

                auto table = tm.findFreeTable();
                if (!table) break;

                player->setTableId(table->getId());
                table->addPlayer(player);
                break;
            }
            case MessageType::Bet:
            case MessageType::Call:
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
            case MessageType::Chat: {
                auto &chat = server.getChatManager();
                chat.addMessage(msg.payload);
                break;
            }
        }
    }
}
