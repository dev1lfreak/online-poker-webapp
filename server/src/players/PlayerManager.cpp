#include "../../include/players/PlayerManager.hpp"

namespace poker {
    std::shared_ptr<Player> PlayerManager::createPlayer(const std::string &nickname) {
        std::lock_guard lock(mutex);
        auto player = std::make_shared<Player>(nextId++, nickname);
        players[player->getId()] = player;
        return player;
    }

    std::shared_ptr<Player> PlayerManager::getPlayer(PlayerId id) {
        std::lock_guard lock(mutex);
        if (players.count(id)) return players[id];
        return nullptr;
    }

    void PlayerManager::removePlayer(PlayerId id) {
        std::lock_guard lock(mutex);
        players.erase(id);
    }
}
