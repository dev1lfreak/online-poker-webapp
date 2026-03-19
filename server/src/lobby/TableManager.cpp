#include "../../include/lobby/TableManager.hpp"
#include <mutex>

namespace poker {
    TableManager::TableManager(boost::asio::io_context &ioContext)
        : io(ioContext) {
    }

    std::shared_ptr<PokerTable> TableManager::createTable() {
        std::lock_guard lock(mutex);
        auto table = std::make_shared<PokerTable>(nextId++, io);
        tables.push_back(table);
        return table;
    }

    std::shared_ptr<PokerTable> TableManager::getTable(int id) {
        std::lock_guard lock(mutex);

        for (auto &t: tables)
            if (t->getId() == id)
                return t;
        return nullptr;
    }

    std::shared_ptr<PokerTable> TableManager::findFreeTable() { // Подумать, что можно сделать с mutex
        std::lock_guard lock(mutex);

        for (auto &t: tables) {
            if (!t->getFull())
                return t;
        }
        return createTable();
    }
}
