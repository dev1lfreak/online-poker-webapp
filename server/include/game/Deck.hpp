#pragma once

#include "Card.hpp"
#include <vector>

namespace poker {
    class Deck {
    public:
        Deck();

        void shuffle();

        Card deal();

    private:
        std::vector<Card> cards;
        size_t index{0};
    };
}
