#include "../../include/network/ConnectionManager.hpp"

namespace poker {
    std::shared_ptr<ClientSession> ConnectionManager::createMockConnection() {
        auto session = std::make_shared<ClientSession>(nextId++);
        sessions.push_back(session);
        return session;
    }

    std::vector<std::shared_ptr<ClientSession> > ConnectionManager::getConnections() {
        return sessions;
    }
}
