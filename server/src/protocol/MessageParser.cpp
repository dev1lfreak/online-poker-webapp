#include "../../include/protocol/MessageParser.hpp"

namespace poker {
    Message MessageParser::parse(const std::string &data) {
        auto obj = boost::json::parse(data).as_object();
        Message msg;
        std::string type = obj.if_contains("type")
                               ? std::string(obj["type"].as_string().c_str())
                               : std::string{};

        std::string payload = obj.if_contains("payload")
                       ? std::string(obj["payload"].as_string().c_str())
                       : std::string{};

        if (type == "login")
            msg.type = MessageType::Login;
        else if (type == "join")
            msg.type = MessageType::JoinTable;
        else if (type == "leave")
            msg.type = MessageType::LeaveTable;
        else if (type == "bet")
            msg.type = MessageType::Bet;
        else if (type == "call")
            msg.type = MessageType::Call;
        else if (type == "check")
            msg.type = MessageType::Check;
        else if (type == "fold")
            msg.type = MessageType::Fold;
        else if (type == "action")
            msg.type = MessageType::Action;
        else if (type == "state")
            msg.type = MessageType::State;
        else
            msg.type = MessageType::Chat;

        if (auto *p = obj.if_contains("playerId")) {
            msg.playerId = static_cast<PlayerId>(p->as_int64());
        }

        if (auto *t = obj.if_contains("tableId")) {
            msg.tableId = static_cast<int>(t->as_int64());
        }

        if (auto *a = obj.if_contains("amount")) {
            msg.amount = static_cast<int>(a->as_int64());
        }

        msg.payload = payload;
        return msg;
    }


    std::string MessageParser::serialize(const Message &msg) {
        boost::json::object obj;
        switch (msg.type) {
            case MessageType::Login: obj["type"] = "login";
                break;
            case MessageType::JoinTable: obj["type"] = "join";
                break;
            case MessageType::LeaveTable: obj["type"] = "leave";
                break;
            case MessageType::Bet: obj["type"] = "bet";
                break;
            case MessageType::Call: obj["type"] = "call";
                break;
            case MessageType::Check: obj["type"] = "check";
                break;
            case MessageType::Fold: obj["type"] = "fold";
                break;
            case MessageType::Chat: obj["type"] = "chat";
                break;
            case MessageType::Action: obj["type"] = "action";
                break;
            case MessageType::State: obj["type"] = "state";
                break;
        }

        if (msg.playerId != 0) {
            obj["playerId"] = static_cast<std::int64_t>(msg.playerId);
        }
        if (msg.tableId >= 0) {
            obj["tableId"] = msg.tableId;
        }

        if (msg.amount > 0) {
            obj["amount"] = msg.amount;
        }

        obj["payload"] = msg.payload;

        return boost::json::serialize(obj);
    }
}
