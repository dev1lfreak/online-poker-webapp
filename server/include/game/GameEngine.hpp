#pragma once

#include "Deck.hpp"
#include "GameState.hpp"
#include "RoundManager.hpp"
#include "PokerRules.hpp"
#include "../players/Player.hpp"
#include <vector>
#include <memory>

namespace poker {
    class GameEngine {
    public:
        void startHand(const std::vector<std::shared_ptr<Player> > &players);

        void dealPrivate();

        void dealFlop();

        void dealTurn();

        void dealRiver();

        GameState getState() const;

        void advanceState();

        const std::vector<Card> &getBoard() const;

        std::shared_ptr<Player> determineWinner();

        RoundManager &getRoundManager();

    private:
        Deck deck;
        std::vector<std::shared_ptr<Player> > players;
        std::vector<Card> board;
        GameState state{GameState::WaitingPlayers};
        RoundManager roundManager;
    };
}
