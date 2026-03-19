#pragma once

#include "Message.hpp"
#include <boost/json.hpp>
#include <string>

namespace poker {
    class MessageParser {
    public:
        static Message parse(const std::string &data);

        static std::string serialize(const Message &msg);
    };
}
