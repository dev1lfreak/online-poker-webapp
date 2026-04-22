#include <gtest/gtest.h>
#include "../../include/game/RoundManager.hpp"
#include "../../include/players/Player.hpp"
#include <memory>
#include <vector>

using namespace poker;

class RoundManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        p1 = std::make_shared<Player>(1, "Alice");
        p2 = std::make_shared<Player>(2, "Bob");
        p3 = std::make_shared<Player>(3, "Charlie");

        p1->setChips(1000);
        p2->setChips(1000);
        p3->setChips(1000);

        players = {p1, p2, p3};
        roundManager = std::make_unique<RoundManager>();
        roundManager->setPlayers(players);
    }

    std::shared_ptr<Player> p1, p2, p3;
    std::vector<std::shared_ptr<Player>> players;
    std::unique_ptr<RoundManager> roundManager;
};

TEST_F(RoundManagerTest, BetDeductsChipsAndAddsToPot) {
    bool success = roundManager->bet(p1->getId(), 100);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(p1->getChips(), 900);
    EXPECT_EQ(roundManager->getPot(), 100);
    EXPECT_EQ(roundManager->getCurrentBet(), 100);
}

TEST_F(RoundManagerTest, CannotBetMoreThanBalance) {
    bool success = roundManager->bet(p2->getId(), 2000);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(p2->getChips(), 1000);
    EXPECT_EQ(roundManager->getPot(), 0);
}

TEST_F(RoundManagerTest, ExactBalanceBetTriggersAllIn) {
    roundManager->bet(p3->getId(), 1000);
    
    EXPECT_EQ(p3->getChips(), 0);
    EXPECT_EQ(p3->getState(), PlayerState::AllIn);
}

TEST_F(RoundManagerTest, BettingNotCompleteUntilAllActivePlayersMatchBet) {
    EXPECT_FALSE(roundManager->isBettingComplete());

    EXPECT_TRUE(roundManager->bet(p1->getId(), 100));
    EXPECT_FALSE(roundManager->isBettingComplete());

    EXPECT_TRUE(roundManager->call(p2->getId()));
    EXPECT_FALSE(roundManager->isBettingComplete());

    EXPECT_TRUE(roundManager->call(p3->getId()));
    EXPECT_TRUE(roundManager->isBettingComplete());
}

TEST_F(RoundManagerTest, RaiseReopensActionForPlayersWhoAlreadyActed) {
    EXPECT_TRUE(roundManager->bet(p1->getId(), 100));
    EXPECT_TRUE(roundManager->call(p2->getId()));
    EXPECT_FALSE(roundManager->isBettingComplete());

    EXPECT_TRUE(roundManager->bet(p3->getId(), 200));
    EXPECT_FALSE(roundManager->isBettingComplete());

    EXPECT_TRUE(roundManager->call(p1->getId())); // +100 to match raise
    EXPECT_TRUE(roundManager->call(p2->getId())); // +100 to match raise
    EXPECT_TRUE(roundManager->isBettingComplete());
}

TEST_F(RoundManagerTest, CheckCompletesRoundWhenNoOutstandingBet) {
    EXPECT_TRUE(roundManager->check(p1->getId()));
    EXPECT_TRUE(roundManager->check(p2->getId()));
    EXPECT_FALSE(roundManager->isBettingComplete());
    EXPECT_TRUE(roundManager->check(p3->getId()));
    EXPECT_TRUE(roundManager->isBettingComplete());
}