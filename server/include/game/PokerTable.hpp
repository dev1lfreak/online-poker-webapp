#pragma once

#include "GameEngine.hpp"
#include "../protocol/Message.hpp"
#include "../util/TurnTimer.hpp"
#include "../network/ConnectionManager.hpp"
#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <string>

namespace poker {
    class PokerTable {
    public:
        PokerTable(int id, boost::asio::io_context &io, ConnectionManager &connectionManager);

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

        void startTurnTimerFor(const std::shared_ptr<Player> &player);

        void disconnectPlayer(PlayerId playerId);
        void requestStartGame();
        void publishState();

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

        int smallBlind{20};
        int bigBlind{40};
        bool gameInProgress{false};

        bool hasLastHandResult_{false};
        std::string lastWinnerNickname_;
        std::string lastWinnerCombination_;
        std::string lastWinnerHoleCards_;
        std::string lastBoardCards_;
        int lastWinAmount_{0};

        PlayerId currentTurnPlayerId{0};
        ConnectionManager &connectionManager;

        void broadcastState() ;
        void broadcastAction(const std::string &action, PlayerId playerId, int amount = 0, const std::string &payload = "");
        void broadcast(const Message &msg);
        std::shared_ptr<Player> playerAtIndex(size_t index) const;
        bool isSmallBlindPlayer(PlayerId id) const;
        bool isBigBlindPlayer(PlayerId id) const;
        bool isBlindPostingTurn(PlayerId id) const;
        int blindAmountFor(PlayerId id) const;
        bool isBotPlayer(const std::shared_ptr<Player> &player) const;
        void processBotTurn(const std::shared_ptr<Player> &player);
        std::string serializeCard(const Card &card) const;
        std::string playerStateText(const std::shared_ptr<Player> &player) const;
    };
}
