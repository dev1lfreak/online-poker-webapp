#include "../../include/game/GameEngine.hpp"
#include "../../include/players/Player.hpp"

namespace poker {
    void GameEngine::startHand(const std::vector<std::shared_ptr<Player> > &p) {
        players = p;

        deck = Deck();
        deck.shuffle();
        board.clear();

        roundManager.setPlayers(players);
        state = GameState::Preflop;
        dealPrivate();
    }

    void GameEngine::dealPrivate() {
        for (auto &p: players) {
            p->clearHand();

            p->giveCard(deck.deal());
            p->giveCard(deck.deal());
        }
    }

    void GameEngine::dealFlop() {
        board.push_back(deck.deal());
        board.push_back(deck.deal());
        board.push_back(deck.deal());
        state = GameState::Flop;
    }

    void GameEngine::dealTurn() {
        board.push_back(deck.deal());
        state = GameState::Turn;
    }

    void GameEngine::dealRiver() {
        board.push_back(deck.deal());
        state = GameState::River;
    }

    GameState GameEngine::getState() const {
        return state;
    }

    void GameEngine::advanceState() {
        switch (state) {
            case GameState::Preflop: dealFlop();
                break;
            case GameState::Flop: dealTurn();
                break;
            case GameState::Turn: dealRiver();
                break;
            case GameState::River: state = GameState::Showdown;
                break;
            default: break;
        }
    }

    const std::vector<Card> &GameEngine::getBoard() const {
        return board;
    }

    std::shared_ptr<Player> GameEngine::determineWinner() {
        return PokerRules::determineWinner(players, board);
    }

    RoundManager &GameEngine::getRoundManager() {
        return roundManager;
    }
}
