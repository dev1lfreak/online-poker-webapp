#include "../../include/core/PokerServer.hpp"
#include "../../include/network/Connection.hpp"
#include <iostream>

namespace poker {

    PokerServer::PokerServer(uint16_t port)
        : io_(),
          signals_(io_),
          acceptor_(io_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
          tableManager_(io_, connectionManager_),
          router_(*this)
    {
        setupSignalHandling();
    }

    PokerServer::~PokerServer() {
        stop();
    }

    void PokerServer::setupSignalHandling() {
        signals_.add(SIGINT);
        signals_.add(SIGTERM);

        signals_.async_wait(
            [this](boost::system::error_code ec, int signo) {
                if (!ec) {
                    std::cout << "\nReceived signal " << signo << ". Shutting down server gracefully..." << std::endl;
                    stop();
                }
            });
    }

    void PokerServer::start() {
        std::cout << "Poker server starting on address " << acceptor_.local_endpoint().address() << ":" << acceptor_.local_endpoint().port()<< std::endl;

        for (int i = 0; i < 2; i++) {
            tableManager_.createTable();
        }

        doAccept();

        unsigned int threadCount = std::thread::hardware_concurrency();
        if (threadCount == 0) threadCount = 4;

        std::cout << "Starting " << threadCount << " worker threads." << std::endl;

        for (unsigned int i = 0; i < threadCount; ++i) {
            threadPool_.emplace_back([this]() {
                io_.run();
            });
        }

        for (auto& t : threadPool_) {
            if (t.joinable()) {
                t.join();
            }
        }

        std::cout << "Server stopped." << std::endl;
    }

    void PokerServer::stop() {
        if (acceptor_.is_open()) {
            boost::system::error_code ec;
            acceptor_.close(ec);
        }

        if (!io_.stopped()) {
            io_.stop();
        }
    }

    void PokerServer::doAccept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!ec) {
                    std::cout << "New connection from: "
                              << socket.remote_endpoint().address().to_string() << std::endl;

                    auto conn = std::make_shared<Connection>(
                        std::move(socket),
                        *this,
                        connectionManager_
                    );
                    conn->start();
                } else {
                    if (ec != boost::asio::error::operation_aborted) {
                        std::cerr << "Accept error: " << ec.message() << std::endl;
                    }
                }

                if (acceptor_.is_open()) {
                    doAccept();
                }
            });
    }

    void PokerServer::handleDisconnect(PlayerId playerId) {
        if (playerId == 0) return;

        auto player = playerManager_.getPlayer(playerId);
        if (!player) return;

        const int tableId = player->getTableId();
        std::cout << "Handling disconnect for player " << playerId
                  << " (table " << tableId << ")" << std::endl;
        if (tableId < 0) return;

        auto table = tableManager_.getTable(tableId);
        if (!table) return;

        table->disconnectPlayer(playerId);
    }
}