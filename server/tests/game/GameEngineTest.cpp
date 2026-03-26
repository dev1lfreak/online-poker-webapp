#include <gtest/gtest.h>
#include "../../include/game/GameEngine.hpp"
#include "../../include/players/Player.hpp"

using namespace poker;

class GameEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        p1 = std::make_shared<Player>(1, "Alice");
        p2 = std::make_shared<Player>(2, "Bob");
        players = {p1, p2};
        engine = std::make_unique<GameEngine>();
        engine->setPlayers(players);
    }

    std::shared_ptr<Player> p1, p2;
    std::vector<std::shared_ptr<Player>> players;
    std::unique_ptr<GameEngine> engine;
};


TEST_F(GameEngineTest, DealPrivateClearsPreviousHand) {
    p1->giveCard({Rank::Ace, Suit::Spades});
    p1->giveCard({Rank::King, Suit::Hearts});
    EXPECT_EQ(p1->getHand().size(), 2);

    engine->dealPrivate();

    EXPECT_EQ(p1->getHand().size(), 2);
    EXPECT_EQ(p2->getHand().size(), 2);
}

TEST_F(GameEngineTest, StateTransitionsAndBoardDealing) {
    EXPECT_TRUE(engine->getBoard().empty());

    engine->dealFlop();
    EXPECT_EQ(engine->getBoard().size(), 3);

    engine->dealTurn();
    EXPECT_EQ(engine->getBoard().size(), 4);

    engine->dealRiver();
    EXPECT_EQ(engine->getBoard().size(), 5);
}