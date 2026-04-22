#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <queue>
#include <mutex>
#include <string>
#include "../protocol/Message.hpp"
#include "../protocol/MessageRouter.hpp"

namespace poker {
    class PokerServer;
    class ConnectionManager;

    class Connection : public std::enable_shared_from_this<Connection> {
    public:
        Connection(boost::asio::ip::tcp::socket socket, PokerServer& server, ConnectionManager& manager);

        void start();

        virtual void send(const Message& msg);

        PlayerId getPlayerId() const { return playerId_; }
        void setPlayerId(PlayerId id) { playerId_ = id; }

    private:
        void doRead();
        void doWrite();
        void handleLogin(const Message& msg);

        boost::asio::ip::tcp::socket socket_;
        PokerServer& server_;
        ConnectionManager& manager_;
        MessageRouter router_;

        boost::asio::streambuf read_buffer_;
        std::queue<std::string> write_queue_;
        std::mutex write_mutex_;

        PlayerId playerId_{0};
    };
}
