#include <gtest/gtest.h>
#include "../../include/game/HandEvaluator.hpp"

using namespace poker;

std::vector<Card> makeHand(std::initializer_list<std::pair<Rank, Suit>> cards) {
    std::vector<Card> res;
    for (const auto& c : cards) {
        res.push_back({c.first, c.second});
    }
    return res;
}

TEST(HandEvaluatorTest, DetectsRoyalFlush) {
    auto hand = makeHand({
        {Rank::Ace, Suit::Spades}, {Rank::King, Suit::Spades},
        {Rank::Queen, Suit::Spades}, {Rank::Jack, Suit::Spades},
        {Rank::Ten, Suit::Spades}, {Rank::Two, Suit::Hearts}, {Rank::Three, Suit::Clubs}
    });
    HandScore score = HandEvaluator::evaluate(hand);
    EXPECT_EQ(score.rank, 9); 
}

TEST(HandEvaluatorTest, DetectsFullHouse) {
    auto hand = makeHand({
        {Rank::Ten, Suit::Spades}, {Rank::Ten, Suit::Hearts},
        {Rank::Ten, Suit::Clubs}, {Rank::Nine, Suit::Spades},
        {Rank::Nine, Suit::Hearts}, {Rank::Two, Suit::Diamonds}, {Rank::Three, Suit::Clubs}
    });
    HandScore score = HandEvaluator::evaluate(hand);
    EXPECT_EQ(score.rank, 6); 
}

TEST(HandEvaluatorTest, DetectsStraight) {
    auto hand = makeHand({
        {Rank::Nine, Suit::Spades}, {Rank::Eight, Suit::Hearts},
        {Rank::Seven, Suit::Clubs}, {Rank::Six, Suit::Diamonds},
        {Rank::Five, Suit::Spades}, {Rank::Ace, Suit::Hearts}, {Rank::King, Suit::Clubs}
    });
    HandScore score = HandEvaluator::evaluate(hand);
    EXPECT_EQ(score.rank, 4); 
}

TEST(HandEvaluatorTest, TiebreakerWorksCorrectly) {
    auto hand1 = makeHand({
        {Rank::Ten, Suit::Spades}, {Rank::Ten, Suit::Hearts},
        {Rank::Ace, Suit::Clubs}, {Rank::Four, Suit::Spades},
        {Rank::Two, Suit::Hearts}
    });
    auto hand2 = makeHand({
        {Rank::Ten, Suit::Diamonds}, {Rank::Ten, Suit::Clubs},
        {Rank::King, Suit::Clubs}, {Rank::Jack, Suit::Spades},
        {Rank::Three, Suit::Hearts}
    });

    HandScore score1 = HandEvaluator::evaluate(hand1);
    HandScore score2 = HandEvaluator::evaluate(hand2);

    EXPECT_EQ(score1.rank, 1);
    EXPECT_EQ(score2.rank, 1);
    EXPECT_TRUE(score1 > score2); 
    EXPECT_FALSE(score2 > score1);
}