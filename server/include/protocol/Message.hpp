#pragma once

#include <string>
#include <cstdint>

namespace poker {
    using PlayerId = std::uint64_t;

    enum class MessageType {
        Login,
        JoinTable,
        PlayWithBots,
        StartGame,
        LeaveTable,
        Disconnect,
        Bet,
        Call,
        Check,
        Fold,
        Chat,
        Action,
        State
    };

    struct Message {
        MessageType type;
        PlayerId playerId{0};
        int tableId{-1};
        int amount{0};
        std::string payload;
    };
}
