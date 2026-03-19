#pragma once

#include "Message.hpp"

namespace poker {
    class PokerServer;

    class MessageRouter {
    public:
        MessageRouter(PokerServer &server);

        void route(const Message &msg);

    private:
        PokerServer &server;
    };
}
