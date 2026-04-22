#include <gtest/gtest.h>

#include "../../include/core/PokerServer.hpp"
#include "../../include/protocol/MessageRouter.hpp"
#include "../../include/players/PlayerManager.hpp"

using namespace poker;

TEST(MessageRouterTest, JoinTableRoutesPlayerToTable) {
    PokerServer server(0);
    auto player = server.getPlayerManager().createPlayer("alice");
    Message msg{MessageType::JoinTable, player->getId(), -1, 0, ""};

    server.getRouter().route(msg);
    server.getIoContext().poll();

    EXPECT_NE(player->getTableId(), -1);

    auto table = server.getTableManager().getTable(player->getTableId());
    ASSERT_NE(table, nullptr);

    bool found = false;
    for (const auto& p : table->getPlayers()) {
        if (p && p->getId() == player->getId()) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST(MessageRouterTest, ChatMessageRoutedToChatManager) {
    PokerServer server(0);
    Message msg{MessageType::Chat, 1, -1, 0, "hello table"};

    server.getRouter().route(msg);
    auto history = server.getChatManager().history();

    ASSERT_EQ(history.size(), 1u);
    EXPECT_EQ(history[0], "hello table");
}

