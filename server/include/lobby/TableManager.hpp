#pragma once

#include "../game/PokerTable.hpp"
#include <vector>
#include <memory>
#include <mutex>
#include <boost/asio.hpp>

namespace poker {
    class TableManager {
    public:
        explicit TableManager(boost::asio::io_context &io);

        std::shared_ptr<PokerTable> createTable();

        std::shared_ptr<PokerTable> getTable(int id);

        std::shared_ptr<PokerTable> findFreeTable();

    private:
        std::vector<std::shared_ptr<PokerTable> > tables;
        std::mutex mutex;
        int nextId{1};
        boost::asio::io_context &io;
    };
}
