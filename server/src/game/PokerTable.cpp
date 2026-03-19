#include "../../include/game/PokerTable.hpp"
#include <algorithm>
#include <iostream>
#include <boost/json.hpp>

namespace poker {
    PokerTable::PokerTable(int id, boost::asio::io_context &io)
        : id(id),
          engine(),
          strand(io.get_executor()),
          turnTimer(io) {
    }

    void PokerTable::addPlayer(std::shared_ptr<Player> player) {
        boost::asio::post(strand, [this,player] {
            players.push_back(player);
            if (players.size() >= 6) isFull = true;
        });
    }

    int PokerTable::getId() const {
        return id;
    }

    const std::vector<std::shared_ptr<Player> > &PokerTable::getPlayers() const {
        return players;
    }

    void PokerTable::broadcastState() {
        boost::json::object obj;
        obj["type"] = "state";
        obj["tableId"] = id;
        auto &rm = engine.getRoundManager();
        obj["pot"] = rm.getPot();
        obj["currentBet"] = rm.getCurrentBet();

        boost::json::array playersArr;
        for (const auto &p: players) {
            boost::json::object po;
            po["playerId"] = static_cast<std::int64_t>(p->getId());
            po["chips"] = p->getChips();
            po["state"] = static_cast<int>(p->getState());
            playersArr.push_back(po);
        }
        obj["players"] = playersArr;

        std::cout << "Broadcast table state: "
                << boost::json::serialize(obj)
                << std::endl;
    }

    void PokerTable::startTurnTimerFor(const std::shared_ptr<Player> &player) {
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

        player->setState(PlayerState::Folded);
        std::cout << "Auto-folded player " << player->getId() << std::endl;

        advanceGameFlow();
    }

    void PokerTable::handleMessage(const Message &msg, std::shared_ptr<Player> player) {
        boost::asio::post(strand, [this, msg, player] {
            if (!player) return;

            auto &rm = engine.getRoundManager();

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
                    break;
                }
                case MessageType::Call: {
                    if (!rm.call(player->getId())) return;
                    int amount = rm.getCurrentBet();
                    std::cout << "Player " << player->getId()
                            << " calls " << amount
                            << " at table " << id << std::endl;
                    break;
                }
                case MessageType::Fold: {
                    rm.fold(player->getId());
                    std::cout << "Player " << player->getId()
                            << " folds at table " << id << std::endl;
                    break;
                }
                default:
                    return;
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
        rotateDealer();
        engine.startHand(players);

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

        broadcastState();
        startGame();
    }

    void PokerTable::startGame() {
        boost::asio::post(strand, [this] {
            if (players.size() > 1) {
                startHand();
            }
        });
    }

    bool PokerTable::getFull() const {
        return isFull;
    }
}
