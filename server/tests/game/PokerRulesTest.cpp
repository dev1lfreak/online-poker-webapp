#include <gtest/gtest.h>
#include "../../include/game/PokerRules.hpp"
#include "../../include/players/Player.hpp"

using namespace poker;

TEST(PokerRulesTest, DetermineWinnerIgnoresFoldedPlayers) {
    auto p1 = std::make_shared<Player>(1, "Alice");
    auto p2 = std::make_shared<Player>(2, "Bob");

    p1->giveCard({Rank::Ace, Suit::Spades});
    p1->giveCard({Rank::Ace, Suit::Hearts});
    p1->setState(PlayerState::Folded);

    p2->giveCard({Rank::Two, Suit::Clubs});
    p2->giveCard({Rank::Three, Suit::Diamonds});
    p2->setState(PlayerState::Active);

    std::vector<Card> board = {
        {Rank::Five, Suit::Spades}, {Rank::Seven, Suit::Hearts},
        {Rank::Nine, Suit::Clubs}, {Rank::Jack, Suit::Diamonds},
        {Rank::King, Suit::Spades}
    };

    std::vector<std::shared_ptr<Player>> players = {p1, p2};

    auto winner = PokerRules::determineWinner(players, board);

    ASSERT_NE(winner, nullptr);
    EXPECT_EQ(winner->getId(), 2);
}