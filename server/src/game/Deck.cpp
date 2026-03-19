#include "../../include/game/Deck.hpp"
#include <algorithm>
#include <random>

namespace poker {
    Deck::Deck() {
        for (int s = 0; s < 4; ++s)
            for (int r = 2; r <= 14; ++r)
                cards.push_back(Card{static_cast<Rank>(r), static_cast<Suit>(s)});
    }

    void Deck::shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(cards.begin(), cards.end(), g);
        index = 0;
    }

    Card Deck::deal() {
        return cards[index++];
    }
}
