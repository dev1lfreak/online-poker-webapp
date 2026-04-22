#pragma once

#include "../game/PokerTable.hpp"
#include "../network/ConnectionManager.hpp"
#include <vector>
#include <memory>
#include <mutex>
#include <boost/asio.hpp>

namespace poker {
    class TableManager {
    public:
        TableManager(boost::asio::io_context &io, ConnectionManager &connectionManager);

        std::shared_ptr<PokerTable> createTable();

        std::shared_ptr<PokerTable> getTable(int id);

        std::shared_ptr<PokerTable> findFreeTable();

    private:
        std::vector<std::shared_ptr<PokerTable> > tables;
        std::mutex mutex;
        int nextId{1};
        boost::asio::io_context &io;
        ConnectionManager &connectionManager;
    };
}
