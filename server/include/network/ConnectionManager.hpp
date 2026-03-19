#pragma once

#include "ClientSession.hpp"
#include <vector>
#include <memory>

namespace poker {
    class ConnectionManager {
    public:
        std::shared_ptr<ClientSession> createMockConnection();

        std::vector<std::shared_ptr<ClientSession> > getConnections();

    private:
        std::vector<std::shared_ptr<ClientSession> > sessions;
        int nextId{1};
    };
}
