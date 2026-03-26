#include "../include/core/PokerServer.hpp"
#include <iostream>

using namespace poker;

int main() {
    try {
        PokerServer server(8080);

        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}