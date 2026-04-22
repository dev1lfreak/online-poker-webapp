#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
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

        bool check(PlayerId id);

        void fold(PlayerId id);

        void disconnect(PlayerId id);

        int getPot() const;

        int getCurrentBet() const;
        int getRoundBet(PlayerId id) const;

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
        std::unordered_map<PlayerId, int> roundBets;
        std::unordered_set<PlayerId> actedPlayers;

        PlayerId findPlayerIndex(PlayerId id) const;

        PlayerId findNextActiveIndex(PlayerId from) const;

    };
}
