#pragma once

#include "Card.hpp"
#include <vector>

namespace poker {
    struct HandScore {
        int rank;
        std::vector<int> tiebreak;

        bool operator>(const HandScore &other) const;
    };

    class HandEvaluator {
    public:
        static HandScore evaluate(const std::vector<Card> &cards);
    };
}
