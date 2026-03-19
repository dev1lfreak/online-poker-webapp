#pragma once

#include "Player.hpp"
#include <unordered_map>
#include <memory>
#include <mutex>

namespace poker {
    class PlayerManager {
    public:
        std::shared_ptr<Player> createPlayer(const std::string &nickname);

        std::shared_ptr<Player> getPlayer(PlayerId id);

        void removePlayer(PlayerId id);

    private:
        std::unordered_map<PlayerId, std::shared_ptr<Player> > players;
        std::mutex mutex;
        PlayerId nextId{1};
    };
}
