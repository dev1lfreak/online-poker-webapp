#pragma once

#include "GameEngine.hpp"
#include "../protocol/Message.hpp"
#include "../util/TurnTimer.hpp"
#include <boost/asio.hpp>
#include <vector>
#include <memory>

namespace poker {
    class PokerTable {
    public:
        PokerTable(int id, boost::asio::io_context &io);

        void addPlayer(std::shared_ptr<Player> player);

        int getId() const;

        const std::vector<std::shared_ptr<Player> > &getPlayers() const;

        void handleMessage(const Message &msg, std::shared_ptr<Player> player);

        void startHand();

        void rotateDealer();

        void startGame();

        void removeBrokePlayers();

        void resolveHand();

        bool getFull() const;

        void startBettingRound();

        void advanceGameFlow();

        void handleTimeout(const std::shared_ptr<Player> &player);

    private:
        int id;
        std::vector<std::shared_ptr<Player> > players;
        GameEngine engine;
        boost::asio::strand<boost::asio::io_context::executor_type> strand;

        TurnTimer turnTimer;
        size_t dealerIndex{0};
        size_t smallBlindIndex{1};
        size_t bigBlindIndex{2};
        bool isFull{false};

        int smallBlind{10};
        int bigBlind{20};

        PlayerId currentTurnPlayerId{0};

        void broadcastState() ;

        void startTurnTimerFor(const std::shared_ptr<Player> &player);
    };
}
