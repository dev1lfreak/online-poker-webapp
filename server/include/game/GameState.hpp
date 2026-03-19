#pragma once

namespace poker {
    enum class GameState {
        WaitingPlayers,
        Preflop,
        Flop,
        Turn,
        River,
        Showdown
    };
}
