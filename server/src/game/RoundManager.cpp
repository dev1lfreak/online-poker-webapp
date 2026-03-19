#include "../../include/game/RoundManager.hpp"

namespace poker {
    void RoundManager::setPlayers(const std::vector<std::shared_ptr<Player> > &p) {
        players = p;
        index = 0;
        pot = 0;
        currentBet = 0;
    }

    std::shared_ptr<Player> RoundManager::currentPlayer() {
        if (players.empty()) return nullptr;
        return players[index];
    }

    PlayerId RoundManager::findPlayerIndex(PlayerId id) const {
        for (size_t i = 0; i < players.size(); ++i) {
            if (players[i]->getId() == id) return i;
        }
        return players.size();
    }

    PlayerId RoundManager::findNextActiveIndex(PlayerId from) const {
        if (players.empty()) return players.size();
        size_t idx = from;
        for (size_t i = 0; i < players.size(); ++i) {
            idx = (idx + 1) % players.size();
            auto &p = players[idx];
            if (p->getState() == PlayerState::Active && p->getChips() > 0)
                return idx;
        }
        return players.size();
    }

    std::shared_ptr<Player> RoundManager::startBettingRound(size_t dealerIndex) {
        currentBet = 0;
        if (players.empty()) return nullptr;

        index = dealerIndex % players.size();
        index = findNextActiveIndex(index);
        if (index == players.size()) return nullptr;
        return players[index];
    }

    std::shared_ptr<Player> RoundManager::nextActiveAfter(PlayerId id) {
        size_t from = findPlayerIndex(id);
        if (from == players.size()) return nullptr;
        index = findNextActiveIndex(from);
        if (index == players.size()) return nullptr;
        return players[index];
    }

    bool RoundManager::bet(PlayerId id, int amount) {
        if (amount <= 0) return false;
        for (auto &p: players) {
            if (p->getId() == id) {
                int chips = p->getChips();
                if (amount > chips) return false;

                p->setChips(chips - amount);
                pot += amount;
                currentBet = amount;

                if (chips <= amount)
                    p->setState(PlayerState::AllIn);
                return true;
            }
        }
        return false;
    }

    bool RoundManager::call(PlayerId id) {
        if (currentBet <= 0) return false;
        for (auto &p: players) {
            if (p->getId() == id) {
                int chips = p->getChips();
                if (chips <= 0) return false;

                int amount = std::min(currentBet, chips);
                p->setChips(chips - amount);
                pot += amount;

                if (chips <= amount)
                    p->setState(PlayerState::AllIn);
                return true;
            }
        }
        return false;
    }

    void RoundManager::fold(PlayerId id) {
        for (auto &p: players)
            if (p->getId() == id)
                p->setState(PlayerState::Folded);
    }

    int RoundManager::getPot() const {
        return pot;
    }

    int RoundManager::getCurrentBet() const {
        return currentBet;
    }

    void RoundManager::resetPot() {
        pot = 0;
    }

    void RoundManager::clearCurrentBet() {
        currentBet = 0;
    }

    bool RoundManager::isBettingComplete() const {
        size_t active = countActivePlayers();
        if (active <= 1) return true;

        for (const auto& p : players) {
            if (p->getState() == PlayerState::Active) {
                if (p->getChips() > 0)
                    return false;
            }
        }
        return true;
    }

    size_t RoundManager::countActivePlayers() const {
        size_t cnt = 0;
        for (const auto &p: players) {
            if (p->getState() == PlayerState::Active && p->getChips() > 0)
                ++cnt;
        }
        return cnt;
    }

    std::shared_ptr<Player> RoundManager::getNextActivePlayer(PlayerId currentId) const {
        if (players.empty()) return nullptr;

        int startIndex = -1;
        for (int i = 0; i < (int)players.size(); ++i) {
            if (players[i]->getId() == currentId) {
                startIndex = i;
                break;
            }
        }

        for (int i = 1; i <= (int)players.size(); ++i) {
            int nextIndex = (startIndex + i) % (int)players.size();
            auto& p = players[nextIndex];

            if (p->getState() == PlayerState::Active && p->getChips() > 0) {
                return p;
            }
        }

        return nullptr;
    }

    std::shared_ptr<Player> RoundManager::lastActivePlayer() const {
        std::shared_ptr<Player> res;
        for (const auto &p: players) {
            if (p->getState() == PlayerState::Active && p->getChips() > 0)
                res = p;
        }
        return res;
    }
}
