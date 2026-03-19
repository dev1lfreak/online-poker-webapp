#include "../../include/network/ClientSession.hpp"
#include <iostream>

namespace poker {
    ClientSession::ClientSession(int id) : id(id) {
    }

    int ClientSession::getId() const {
        return id;
    }

    void ClientSession::send(const std::string &msg) {
        std::cout << "Mock send to client " << id << " : " << msg << std::endl;
    }
}
