#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include "../../include/game/PokerTable.hpp"
#include "../../include/players/Player.hpp"
#include "../../include/network/ConnectionManager.hpp"

using namespace poker;

TEST(PokerTableTest, TableSetsIsFullFlagAfter6Players) {
    boost::asio::io_context io;
    ConnectionManager connectionManager;
    PokerTable table(1, io, connectionManager);

    EXPECT_FALSE(table.getFull());

    for (int i = 1; i <= 6; ++i) {
        auto p = std::make_shared<Player>(i, "Player" + std::to_string(i));
        table.addPlayer(p);
    }

    io.run();

    EXPECT_TRUE(table.getFull());
}