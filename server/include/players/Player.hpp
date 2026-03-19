#pragma once

#include <string>
#include <vector>
#include "../game/Card.hpp"

namespace poker {
    using PlayerId = uint64_t;

    enum class PlayerState {
        Active,
        Folded,
        AllIn,
        Disconnected
    };

    class Player {
    public:
        Player(PlayerId id, std::string nickname);

        PlayerId getId() const;

        const std::string &getNickname() const;

        void setChips(int chips);

        int getChips() const;

        void giveCard(const Card &card);

        void clearHand();

        const std::vector<Card> &getHand() const;

        void setState(PlayerState s);

        PlayerState getState() const;

        void setTableId(int table);

        int getTableId() const;

    private:
        PlayerId id;
        std::string nickname;

        int chips{1000};
        std::vector<Card> hand;
        PlayerState state{PlayerState::Active};
        int tableId{-1};
    };
}
