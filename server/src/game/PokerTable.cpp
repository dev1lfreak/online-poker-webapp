#include "../../include/game/PokerTable.hpp"
#include "../../include/game/HandEvaluator.hpp"
#include <algorithm>
#include <iostream>
#include <boost/json.hpp>

namespace poker {
    namespace {
        bool startsWith(const std::string &value, const std::string &prefix) {
            return value.rfind(prefix, 0) == 0;
        }

        std::string suitToString(const poker::Suit suit) {
            switch (suit) {
                case poker::Suit::Hearts: return "H";
                case poker::Suit::Diamonds: return "D";
                case poker::Suit::Clubs: return "C";
                case poker::Suit::Spades: return "S";
            }
            return "?";
        }

        std::string rankToString(const poker::Rank rank) {
            switch (rank) {
                case poker::Rank::Two: return "2";
                case poker::Rank::Three: return "3";
                case poker::Rank::Four: return "4";
                case poker::Rank::Five: return "5";
                case poker::Rank::Six: return "6";
                case poker::Rank::Seven: return "7";
                case poker::Rank::Eight: return "8";
                case poker::Rank::Nine: return "9";
                case poker::Rank::Ten: return "T";
                case poker::Rank::Jack: return "J";
                case poker::Rank::Queen: return "Q";
                case poker::Rank::King: return "K";
                case poker::Rank::Ace: return "A";
            }
            return "?";
        }

        std::string handRankToText(int rank) {
            switch (rank) {
                case 9: return "Royal Flush";
                case 8: return "Straight Flush";
                case 7: return "Four of a Kind";
                case 6: return "Full House";
                case 5: return "Flush";
                case 4: return "Straight";
                case 3: return "Three of a Kind";
                case 2: return "Two Pair";
                case 1: return "One Pair";
                default: return "High Card";
            }
        }

        std::string joinCards(const std::vector<poker::Card> &cards) {
            std::string out;
            for (size_t i = 0; i < cards.size(); ++i) {
                if (!out.empty()) out += " ";
                out += "[" + rankToString(cards[i].rank) + suitToString(cards[i].suit) + "]";
            }
            return out;
        }
    }

    PokerTable::PokerTable(int id, boost::asio::io_context &io, ConnectionManager &connectionManager)
        : id(id),
          engine(),
          strand(io.get_executor()),
          turnTimer(io),
          connectionManager(connectionManager) {
    }

    void PokerTable::addPlayer(std::shared_ptr<Player> player) {
        boost::asio::post(strand, [this,player] {
            if (!isFull) {
                players.push_back(player);
                player->setTableId(id);
                if (players.size() >= 6) {
                    isFull = true;
                }
            }
        });
    }

    int PokerTable::getId() const {
        return id;
    }

    const std::vector<std::shared_ptr<Player> > &PokerTable::getPlayers() const {
        return players;
    }

    std::shared_ptr<Player> PokerTable::playerAtIndex(size_t index) const {
        if (players.empty()) return nullptr;
        return players[index % players.size()];
    }

    bool PokerTable::isSmallBlindPlayer(PlayerId id) const {
        const auto player = playerAtIndex(smallBlindIndex);
        return player && player->getId() == id;
    }

    bool PokerTable::isBigBlindPlayer(PlayerId id) const {
        const auto player = playerAtIndex(bigBlindIndex);
        return player && player->getId() == id;
    }

    bool PokerTable::isBlindPostingTurn(PlayerId id) const {
        return engine.getState() == GameState::Preflop &&
               (isSmallBlindPlayer(id) || isBigBlindPlayer(id));
    }

    int PokerTable::blindAmountFor(PlayerId id) const {
        if (isSmallBlindPlayer(id)) return smallBlind;
        if (isBigBlindPlayer(id)) return bigBlind;
        return 0;
    }

    void PokerTable::broadcastState() {
        boost::json::object obj;
        obj["event"] = "state_snapshot";
        obj["tableId"] = id;
        auto &rm = engine.getRoundManager();
        obj["pot"] = rm.getPot();
        obj["currentBet"] = rm.getCurrentBet();
        obj["currentTurnPlayerId"] = static_cast<std::int64_t>(currentTurnPlayerId);
        obj["gameState"] = static_cast<int>(engine.getState());
        obj["gameInProgress"] = gameInProgress;
        obj["canStartGame"] = players.size() > 1 && !gameInProgress;
        obj["smallBlindAmount"] = smallBlind;
        obj["bigBlindAmount"] = bigBlind;
        if (!players.empty()) {
            const auto smallBlindPlayer = playerAtIndex(smallBlindIndex);
            const auto bigBlindPlayer = playerAtIndex(bigBlindIndex);
            obj["smallBlindPlayerId"] = smallBlindPlayer ? static_cast<std::int64_t>(smallBlindPlayer->getId()) : 0;
            obj["bigBlindPlayerId"] = bigBlindPlayer ? static_cast<std::int64_t>(bigBlindPlayer->getId()) : 0;
        }
        obj["hasLastHandResult"] = hasLastHandResult_;
        if (hasLastHandResult_) {
            obj["lastWinnerNickname"] = lastWinnerNickname_;
            obj["lastWinnerCombination"] = lastWinnerCombination_;
            obj["lastWinnerHoleCards"] = lastWinnerHoleCards_;
            obj["lastBoardCards"] = lastBoardCards_;
            obj["lastWinAmount"] = lastWinAmount_;
        }

        boost::json::array playersArr;
        for (const auto &p: players) {
            boost::json::object po;
            po["playerId"] = static_cast<std::int64_t>(p->getId());
            po["nickname"] = p->getNickname();
            po["chips"] = p->getChips();
            po["state"] = static_cast<int>(p->getState());
            po["stateText"] = playerStateText(p);
            po["isDealer"] = players.size() > 1 && players[dealerIndex % players.size()]->getId() == p->getId();
            po["isBot"] = isBotPlayer(p);
            playersArr.push_back(po);
        }
        obj["players"] = playersArr;

        boost::json::array boardArr;
        for (const auto &card : engine.getBoard()) {
            boardArr.push_back(boost::json::value(serializeCard(card)));
        }
        obj["board"] = boardArr;
        Message stateMessage;
        stateMessage.type = MessageType::State;
        stateMessage.tableId = id;
        stateMessage.payload = boost::json::serialize(obj);
        for (const auto &player : players) {
            if (!player) continue;
            if (player->getState() == PlayerState::Disconnected) continue;
            auto personalized = obj;
            boost::json::array myCardsArr;
            for (const auto &card : player->getHand()) {
                myCardsArr.push_back(boost::json::value(serializeCard(card)));
            }
            personalized["myCards"] = myCardsArr;
            const int myRoundBet = rm.getRoundBet(player->getId());
            personalized["myRoundBet"] = myRoundBet;
            personalized["callAmount"] = std::max(0, rm.getCurrentBet() - myRoundBet);

            Message personalizedState;
            personalizedState.type = MessageType::State;
            personalizedState.tableId = id;
            personalizedState.playerId = player->getId();
            personalizedState.payload = boost::json::serialize(personalized);
            connectionManager.sendTo(player->getId(), personalizedState);
        }
    }

    // void PokerTable::broadcast(const Message& msg) {
    //     // Предполагаем, что у стола есть ссылка на connectionManager_
    //     for (const auto& player : players) {
    //         // Рассылаем всем, кто сидит за столом и не отключился
    //         if (player && player->getState() != PlayerState::Disconnected) {
    //             connectionManager_.sendTo(player->getId(), msg);
    //         }
    //     }
    // }
    //
    // // Пример использования: Игрок сделал ставку
    // void PokerTable::handleBet(PlayerId id, int amount) {
    //     // ... логика снятия фишек ...
    //
    //     // Уведомляем всех за столом
    //     Message updateMsg;
    //     updateMsg.type = MessageType::Bet;
    //     updateMsg.playerId = id;
    //     updateMsg.tableId = this->tableId;
    //     updateMsg.amount = amount;
    //
    //     broadcast(updateMsg);
    // }

    void PokerTable::startTurnTimerFor(const std::shared_ptr<Player> &player) {
        if (isBotPlayer(player)) {
            currentTurnPlayerId = player->getId();
            broadcastState();
            turnTimer.start(5, [this, player]() {
                boost::asio::post(strand, [this, player]() {
                    if (currentTurnPlayerId == player->getId()) {
                        processBotTurn(player);
                    }
                });
            });
            return;
        }

        currentTurnPlayerId = player->getId();

        turnTimer.start(30, [this, player]() {
            boost::asio::post(strand, [this, player]() {
                if (currentTurnPlayerId == player->getId()) {
                    handleTimeout(player);
                }
            });
        });
    }

    void PokerTable::handleTimeout(const std::shared_ptr<Player> &player) {
        std::cout << "Player " << player->getId() << " timed out." << std::endl;
        auto &rm = engine.getRoundManager();
        if (isBlindPostingTurn(player->getId())) {
            const int expectedAmount = blindAmountFor(player->getId());
            const int amount = std::min(expectedAmount, player->getChips());
            if (amount > 0 && rm.bet(player->getId(), amount)) {
                const std::string actionName = isSmallBlindPlayer(player->getId()) ? "small_blind" : "big_blind";
                broadcastAction(actionName, player->getId(), amount);
                std::cout << "Auto-posted blind for player " << player->getId() << std::endl;
            } else {
                rm.fold(player->getId());
                broadcastAction("timeout_fold", player->getId());
                std::cout << "Auto-folded player " << player->getId() << std::endl;
            }
        } else {
            rm.fold(player->getId());
            broadcastAction("timeout_fold", player->getId());
            std::cout << "Auto-folded player " << player->getId() << std::endl;
        }

        advanceGameFlow();
    }

    void PokerTable::handleMessage(const Message &msg, std::shared_ptr<Player> player) {
        boost::asio::post(strand, [this, msg, player] {
            if (!player) return;

            auto &rm = engine.getRoundManager();
            const bool blindPostingTurn = isBlindPostingTurn(player->getId());

            if (blindPostingTurn) {
                if (msg.type == MessageType::LeaveTable) {
                    disconnectPlayer(player->getId());
                    return;
                }

                if (msg.type != MessageType::Bet) {
                    return;
                }

                const int expectedAmount = blindAmountFor(player->getId());
                const int amount = std::min(expectedAmount, player->getChips());
                if (amount <= 0 || (msg.amount != expectedAmount && msg.amount != amount)) {
                    std::cout << "Invalid blind bet from player " << player->getId() << std::endl;
                    return;
                }

                if (!rm.bet(player->getId(), amount)) {
                    std::cout << "Failed blind bet from player " << player->getId() << std::endl;
                    return;
                }

                const std::string actionName = isSmallBlindPlayer(player->getId()) ? "small_blind" : "big_blind";
                std::cout << "Player " << player->getId()
                        << " posts " << actionName
                        << " " << amount
                        << " at table " << id << std::endl;
                broadcastAction(actionName, player->getId(), amount);
            } else {
                switch (msg.type) {
                    case MessageType::Bet: {
                        int amount = msg.amount;
                        if (!rm.bet(player->getId(), amount)) {
                            std::cout << "Invalid bet from player " << player->getId() << std::endl;
                            return;
                        }
                        std::cout << "Player " << player->getId()
                                << " bets " << amount
                                << " at table " << id << std::endl;
                        broadcastAction("bet", player->getId(), amount);
                        break;
                    }
                    case MessageType::Call: {
                        if (!rm.call(player->getId())) return;
                        int amount = rm.getCurrentBet();
                        std::cout << "Player " << player->getId()
                                << " calls " << amount
                                << " at table " << id << std::endl;
                        broadcastAction("call", player->getId(), amount);
                        break;
                    }
                    case MessageType::Check: {
                        if (!rm.check(player->getId())) return;
                        std::cout << "Player " << player->getId()
                                << " checks at table " << id << std::endl;
                        broadcastAction("check", player->getId());
                        break;
                    }
                    case MessageType::Fold: {
                        rm.fold(player->getId());
                        std::cout << "Player " << player->getId()
                                << " folds at table " << id << std::endl;
                        broadcastAction("fold", player->getId());
                        break;
                    }
                    case MessageType::LeaveTable: {
                        disconnectPlayer(player->getId());
                        break;
                    }
                    default:
                        return;
                }
            }

            if (rm.isBettingComplete()) {
                advanceGameFlow();
            } else {
                auto next = rm.nextActiveAfter(player->getId());

                if (next) {
                    startTurnTimerFor(next);
                } else {
                    advanceGameFlow();
                }
            }
            broadcastState();
        });
    }

    void PokerTable::startHand() {
        gameInProgress = true;
        rotateDealer();
        engine.startHand();

        startBettingRound();
    }

    void PokerTable::startBettingRound() {
        auto &rm = engine.getRoundManager();
        rm.clearCurrentBet();

        auto first = rm.startBettingRound(dealerIndex);
        if (!first) {
            resolveHand();
            return;
        }

        broadcastState();
        startTurnTimerFor(first);
    }

    void PokerTable::advanceGameFlow() {
        turnTimer.cancel();

        auto &rm = engine.getRoundManager();

        if (rm.isBettingComplete()) {
            if (engine.getState() == GameState::River) {
                resolveHand();
            } else {
                engine.advanceState();
                startBettingRound();
            }
        } else {
            auto nextPlayer = rm.getNextActivePlayer(currentTurnPlayerId);
            if (nextPlayer) {
                startTurnTimerFor(nextPlayer);
                broadcastState();
            } else {
                resolveHand();
            }
        }
    }

    void PokerTable::rotateDealer() {
        dealerIndex = (dealerIndex + 1) % players.size();
        smallBlindIndex = (dealerIndex + 1) % players.size();
        bigBlindIndex = (dealerIndex + 2) % players.size();
    }

    void PokerTable::removeBrokePlayers() {
        players.erase(
            std::remove_if(players.begin(), players.end(),
                           [](auto &p) { return p->getChips() == 0; }),
            players.end()
        );
    }

    void PokerTable::resolveHand() {
        turnTimer.cancel();

        auto &rm = engine.getRoundManager();
        auto activeCount = rm.countActivePlayers();
        std::shared_ptr<Player> winner;

        if (activeCount == 0) {
            std::cout << "No active players at table " << id << ", pot returned to house" << std::endl;
        } else if (activeCount == 1) {
            winner = rm.lastActivePlayer();
        } else {
            winner = engine.determineWinner();
        }

        int pot = rm.getPot();
        if (winner) {
            auto winnerCards = winner->getHand();
            winnerCards.insert(winnerCards.end(), engine.getBoard().begin(), engine.getBoard().end());
            const auto score = HandEvaluator::evaluate(winnerCards);
            hasLastHandResult_ = true;
            lastWinnerNickname_ = winner->getNickname();
            lastWinnerCombination_ = handRankToText(score.rank);
            lastWinnerHoleCards_ = joinCards(winner->getHand());
            lastBoardCards_ = joinCards(engine.getBoard());
            lastWinAmount_ = pot;
        } else {
            hasLastHandResult_ = false;
            lastWinnerNickname_.clear();
            lastWinnerCombination_.clear();
            lastWinnerHoleCards_.clear();
            lastBoardCards_.clear();
            lastWinAmount_ = 0;
        }

        if (winner && pot > 0) {
            winner->setChips(winner->getChips() + pot);
            std::cout << "Winner at table " << id
                    << " is player " << winner->getId()
                    << " wins pot " << pot << std::endl;
        }

        rm.resetPot();
        rm.clearCurrentBet();

        for (auto &p: players) {
            if (p->getChips() > 0) {
                p->setState(PlayerState::Active);
            } else {
                p->setState(PlayerState::Folded);
            }
        }

        gameInProgress = false;
        currentTurnPlayerId = 0;
        broadcastState();
    }

    void PokerTable::startGame() {
        boost::asio::post(strand, [this] {
            if (players.size() > 1 && !gameInProgress) {
                engine.setPlayers(players);
                startHand();
            }
        });
    }

    bool PokerTable::getFull() const {
        return isFull;
    }

    void PokerTable::disconnectPlayer(PlayerId playerId) {
        boost::asio::post(strand, [this, playerId]() {
            auto &rm = engine.getRoundManager();

            auto it = std::find_if(players.begin(), players.end(),
                                   [playerId](const std::shared_ptr<Player> &p) {
                                       return p && p->getId() == playerId;
                                   });
            if (it == players.end()) return;

            (*it)->setState(PlayerState::Disconnected);
            (*it)->setTableId(-1);
            rm.disconnect(playerId);
            players.erase(it);
            isFull = players.size() >= 6;

            broadcastAction("disconnect", playerId);
            broadcastState();

            if (currentTurnPlayerId == playerId) {
                advanceGameFlow();
            }
        });
    }

    void PokerTable::requestStartGame() {
        startGame();
    }

    void PokerTable::publishState() {
        boost::asio::post(strand, [this]() {
            broadcastState();
        });
    }

    void PokerTable::broadcastAction(const std::string &action, PlayerId playerId, int amount, const std::string &payload) {
        boost::json::object obj;
        obj["event"] = action;
        obj["tableId"] = id;
        obj["playerId"] = static_cast<std::int64_t>(playerId);
        if (amount > 0) {
            obj["amount"] = amount;
        }
        if (!payload.empty()) {
            obj["payload"] = payload;
        }

        Message actionMessage;
        actionMessage.type = MessageType::Action;
        actionMessage.playerId = playerId;
        actionMessage.tableId = id;
        actionMessage.amount = amount;
        actionMessage.payload = boost::json::serialize(obj);
        broadcast(actionMessage);
    }

    void PokerTable::broadcast(const Message &msg) {
        for (const auto &player : players) {
            if (!player) continue;
            if (player->getState() == PlayerState::Disconnected) continue;
            connectionManager.sendTo(player->getId(), msg);
        }
    }

    bool PokerTable::isBotPlayer(const std::shared_ptr<Player> &player) const {
        return player && startsWith(player->getNickname(), "BOT_");
    }

    void PokerTable::processBotTurn(const std::shared_ptr<Player> &player) {
        boost::asio::post(strand, [this, player]() {
            if (!player) return;

            auto &rm = engine.getRoundManager();
            const bool blindPostingTurn = isBlindPostingTurn(player->getId());
            bool actionDone = false;
            if (blindPostingTurn) {
                const int amount = std::min(blindAmountFor(player->getId()), player->getChips());
                if (amount > 0) {
                    actionDone = rm.bet(player->getId(), amount);
                    if (actionDone) {
                        const std::string actionName = isSmallBlindPlayer(player->getId()) ? "small_blind" : "big_blind";
                        broadcastAction(actionName, player->getId(), amount);
                    }
                }
            } else if (rm.getCurrentBet() > 0) {
                actionDone = rm.call(player->getId());
                if (actionDone) {
                    broadcastAction("call", player->getId(), rm.getCurrentBet());
                }
            } else {
                actionDone = rm.check(player->getId());
                if (actionDone) {
                    broadcastAction("check", player->getId());
                }
            }

            if (!actionDone) {
                rm.fold(player->getId());
                broadcastAction("fold", player->getId());
            }

            if (rm.isBettingComplete()) {
                advanceGameFlow();
            } else {
                auto next = rm.nextActiveAfter(player->getId());
                if (next) {
                    startTurnTimerFor(next);
                } else {
                    advanceGameFlow();
                }
            }
            broadcastState();
        });
    }

    std::string PokerTable::serializeCard(const Card &card) const {
        return rankToString(card.rank) + suitToString(card.suit);
    }

    std::string PokerTable::playerStateText(const std::shared_ptr<Player> &player) const {
        switch (player->getState()) {
            case PlayerState::Active: return "Active";
            case PlayerState::Folded: return "Folded";
            case PlayerState::AllIn: return "AllIn";
            case PlayerState::Check: return "Check";
            case PlayerState::Bet: return "Bet";
            case PlayerState::Disconnected: return "Disconnected";
        }
        return "Unknown";
    }
}
