#pragma once

#include <vector>
#include <memory>
#include "../players/Player.hpp"

namespace poker {
    class RoundManager {
    public:
        void setPlayers(const std::vector<std::shared_ptr<Player> > &players);

        std::shared_ptr<Player> currentPlayer();

        std::shared_ptr<Player> startBettingRound(size_t dealerIndex);

        std::shared_ptr<Player> nextActiveAfter(PlayerId id);

        bool bet(PlayerId id, int amount);

        bool call(PlayerId id);

        void fold(PlayerId id);

        int getPot() const;

        int getCurrentBet() const;

        void resetPot();

        void clearCurrentBet();

        bool isBettingComplete() const;

        size_t countActivePlayers() const;

        std::shared_ptr<Player> lastActivePlayer() const;

        std::shared_ptr<Player> getNextActivePlayer(PlayerId currentId) const;

    private:
        std::vector<std::shared_ptr<Player> > players;
        size_t index{0};
        int pot{0};
        int currentBet{0};

        PlayerId findPlayerIndex(PlayerId id) const;

        PlayerId findNextActiveIndex(PlayerId from) const;

    };
}
