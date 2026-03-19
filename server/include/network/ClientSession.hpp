#pragma once

#include <string>
#include <memory>

namespace poker {
    class ClientSession {
    public:
        ClientSession(int id);

        int getId() const;

        void send(const std::string &msg);

    private:
        int id;
    };
}
