#pragma once

#include <cstdint>

namespace poker {
    enum class Suit { Hearts, Diamonds, Clubs, Spades };

    enum class Rank {
        Two = 2, Three, Four, Five, Six, Seven,
        Eight, Nine, Ten, Jack, Queen, King, Ace
    };

    struct Card {
        Rank rank;
        Suit suit;
    };
}
