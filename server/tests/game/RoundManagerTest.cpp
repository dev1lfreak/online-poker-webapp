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

// // Проверка завершения круга торговли (все уравняли)
// TEST_F(RoundManagerTest, BettingIsCompleteWhenAllMatchCall) {
//     roundManager->bet(p1->getId(), 100); // Алиса ставит 100
//     EXPECT_FALSE(roundManager->isBettingComplete());
//
//     roundManager->bet(p2->getId(), 100); // Боб коллирует
//     EXPECT_FALSE(roundManager->isBettingComplete());
//
//     roundManager->bet(p3->getId(), 100); // Чарли коллирует
//     // Теперь все внесли равную долю в банк
//     EXPECT_TRUE(roundManager->isBettingComplete());
//     EXPECT_EQ(roundManager->getPot(), 300);
// }
//
// // Проверка, что фолд исключает игрока из требований к окончанию торгов
// TEST_F(RoundManagerTest, FoldedPlayersAreIgnoredInBettingComplete) {
//     roundManager->bet(p1->getId(), 200);
//
//     roundManager->fold(p2->getId()); // Боб сбрасывает
//     EXPECT_EQ(p2->getState(), PlayerState::Folded);
//
//     roundManager->bet(p3->getId(), 200); // Чарли коллирует
//
//     // Торговля должна завершиться, так как Боб сбросил, а Алиса и Чарли уравнялись
//     EXPECT_TRUE(roundManager->isBettingComplete());
// }