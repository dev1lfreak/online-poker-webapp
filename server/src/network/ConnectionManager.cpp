#include "../../include/network/ConnectionManager.hpp"
#include "../../include/network/Connection.hpp"

namespace poker {
    void ConnectionManager::add(std::shared_ptr<Connection> conn) {
        std::lock_guard<std::mutex> lock(mutex_);
        unauthenticated_.insert(conn);
    }

    void ConnectionManager::remove(std::shared_ptr<Connection> conn) {
        std::lock_guard<std::mutex> lock(mutex_);
        unauthenticated_.erase(conn);
        if (conn->getPlayerId() != 0) {
            authenticated_.erase(conn->getPlayerId());
        }
    }

    void ConnectionManager::authenticate(std::shared_ptr<Connection> conn, PlayerId playerId) {
        std::lock_guard<std::mutex> lock(mutex_);
        unauthenticated_.erase(conn);
        authenticated_[playerId] = conn;
    }

    void ConnectionManager::sendTo(PlayerId playerId, const Message& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = authenticated_.find(playerId);
        if (it != authenticated_.end()) {
            it->second->send(msg);
        }
    }

    void ConnectionManager::broadcastAll(const Message& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& pair : authenticated_) {
            pair.second->send(msg);
        }
    }
}