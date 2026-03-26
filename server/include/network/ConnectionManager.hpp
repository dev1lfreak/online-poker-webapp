#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "../protocol/Message.hpp"

namespace poker {
    class Connection;

    class ConnectionManager {
    public:
        void add(std::shared_ptr<Connection> conn);

        void remove(std::shared_ptr<Connection> conn);

        void authenticate(std::shared_ptr<Connection> conn, PlayerId playerId);

        void sendTo(PlayerId playerId, const Message& msg);

        void broadcastAll(const Message& msg);

    private:
        std::mutex mutex_;
        std::unordered_set<std::shared_ptr<Connection>> unauthenticated_;
        std::unordered_map<PlayerId, std::shared_ptr<Connection>> authenticated_;
    };
}