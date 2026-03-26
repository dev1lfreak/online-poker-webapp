#include "../../include/network/Connection.hpp"
#include "../../include/network/ConnectionManager.hpp"
#include "../../include/core/PokerServer.hpp"
#include "../../include/protocol/MessageParser.hpp"
#include "../../include/protocol/MessageRouter.hpp"
#include <iostream>

namespace poker {
    Connection::Connection(boost::asio::ip::tcp::socket socket, PokerServer& server, ConnectionManager& manager)
        : socket_(std::move(socket)), server_(server), manager_(manager), router_(server) {}

    void Connection::start() {
        manager_.add(shared_from_this());
        doRead();
    }

    void Connection::send(const Message& msg) {
        std::string data = MessageParser::serialize(msg) + "\n";
        bool write_in_progress;

        {
            std::lock_guard<std::mutex> lock(write_mutex_);
            write_in_progress = !write_queue_.empty();
            write_queue_.push(data);
        }

        if (!write_in_progress) {
            doWrite();
        }
    }

    void Connection::doRead() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, read_buffer_, '\n',
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::istream is(&read_buffer_);
                    std::string line;
                    std::getline(is, line);

                    try {
                        Message msg = MessageParser::parse(line);

                        if (msg.type == MessageType::Login) {
                            handleLogin(msg);
                        } else {
                            msg.playerId = this->playerId_;

                            if (msg.playerId != 0) {
                                router_.route(msg);
                            }
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Parse error: " << e.what() << std::endl;
                    }

                    doRead();
                } else {
                    manager_.remove(self);
                }
            });
    }

    void Connection::doWrite() {
        auto self(shared_from_this());
        std::lock_guard<std::mutex> lock(write_mutex_);

        boost::asio::async_write(socket_,
            boost::asio::buffer(write_queue_.front()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    std::lock_guard<std::mutex> lock(write_mutex_);
                    write_queue_.pop();
                    if (!write_queue_.empty()) {
                        doWrite();
                    }
                } else {
                    manager_.remove(self);
                }
            });
    }

    void Connection::handleLogin(const Message& msg) {
        auto player = server_.getPlayerManager().createPlayer(msg.payload);
        playerId_ = player->getId();
        manager_.authenticate(shared_from_this(), playerId_);

        Message response;
        response.type = MessageType::Login;
        response.playerId = playerId_;
        send(response);
    }
}