#include <gtest/gtest.h>
#include "../../include/players/PlayerManager.hpp"
#include <thread>
#include <vector>

using namespace poker;

TEST(PlayerManagerTest, ConcurrentCreatePlayerCreatesCorrectAmountAndIds) {
    PlayerManager pm;
    const int numThreads = 10;
    constexpr int playersPerThread = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&pm, i, playersPerThread]() {
            for (int j = 0; j < playersPerThread; ++j) {
                pm.createPlayer("Player_" + std::to_string(i) + "_" + std::to_string(j));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    int totalExpectedPlayers = numThreads * playersPerThread;

    EXPECT_NE(pm.getPlayer(totalExpectedPlayers), nullptr);
    EXPECT_EQ(pm.getPlayer(totalExpectedPlayers + 1), nullptr);
}