#include "../../include/players/Player.hpp"

namespace poker {
    Player::Player(PlayerId id, std::string nickname)
        : id(id), nickname(std::move(nickname)) {
    }

    PlayerId Player::getId() const {
        return id;
    }

    const std::string &Player::getNickname() const {
        return nickname;
    }

    void Player::setChips(int c) {
        chips = c;
    }

    int Player::getChips() const {
        return chips;
    }

    void Player::giveCard(const Card &card) {
        hand.push_back(card);
    }

    void Player::clearHand() {
        hand.clear();
    }

    const std::vector<Card> &Player::getHand() const {
        return hand;
    }

    void Player::setState(PlayerState s) {
        state = s;
    }

    PlayerState Player::getState() const {
        return state;
    }

    void Player::setTableId(int table) {
        tableId = table;
    }

    int Player::getTableId() const {
        return tableId;
    }
}
