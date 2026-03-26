#include <gtest/gtest.h>
#include "../../include/game/Deck.hpp"

using namespace poker;

TEST(DeckTest, InitializationCreates52Cards) {
    Deck deck;
    int count = 0;

    for (int i = 0; i < 52; ++i) {
        Card c = deck.deal();
        EXPECT_GE(static_cast<int>(c.rank), 2);
        EXPECT_LE(static_cast<int>(c.rank), 14);
        count++;
    }
    EXPECT_EQ(count, 52);
}

TEST(DeckTest, ShuffleChangesCardOrder) {
    Deck deck1;
    Deck deck2;
    deck2.shuffle();

    bool differenceFound = false;
    for(int i = 0; i < 52; ++i) {
        Card c1 = deck1.deal();
        Card c2 = deck2.deal();
        if (c1.rank != c2.rank || c1.suit != c2.suit) {
            differenceFound = true;
            break;
        }
    }
    EXPECT_TRUE(differenceFound);
}