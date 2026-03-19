#pragma once

#include "../players/Player.hpp"
#include "HandEvaluator.hpp"
#include <vector>
#include <memory>

namespace poker {
    class PokerRules {
    public:
        static std::shared_ptr<Player> determineWinner(
            const std::vector<std::shared_ptr<Player> > &players,
            const std::vector<Card> &board
        );
    };
}
