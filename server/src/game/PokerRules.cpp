#include "../../include/game/PokerRules.hpp"

namespace poker {
    std::shared_ptr<Player> PokerRules::determineWinner(
        const std::vector<std::shared_ptr<Player> > &players,
        const std::vector<Card> &board) {
        HandScore best{-1, {}};
        std::shared_ptr<Player> winner;

        for (auto &p: players) {
            if (p->getState() == PlayerState::Folded)
                continue;

            auto cards = p->getHand();
            cards.insert(cards.end(), board.begin(), board.end());
            auto score = HandEvaluator::evaluate(cards);

            if (score > best) {
                best = score;
                winner = p;
            }
        }
        return winner;
    }
}
