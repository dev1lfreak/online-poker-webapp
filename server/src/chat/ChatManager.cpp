#include "../../include/chat/ChatManager.hpp"

namespace poker {

    void ChatManager::addMessage(const std::string& msg) {
        std::lock_guard lock(mutex);
        messages.push_back(msg);
    }

    std::vector<std::string> ChatManager::history() {
        std::lock_guard lock(mutex);
        return messages;
    }
}