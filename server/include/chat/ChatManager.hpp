#pragma once

#include <string>
#include <vector>
#include <mutex>

namespace poker {
    class ChatManager {
    public:
        void addMessage(const std::string &msg);

        std::vector<std::string> history();

    private:
        std::vector<std::string> messages;
        std::mutex mutex;
    };
}
