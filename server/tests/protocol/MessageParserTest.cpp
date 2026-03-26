#include <gtest/gtest.h>
#include "../../include/protocol/MessageParser.hpp"
#include "../../include/protocol/Message.hpp"

using namespace poker;

TEST(MessageParserTest, ParsesValidBetJson) {
    std::string jsonStr = R"({"type": "bet", "playerId": 42, "tableId": 1, "amount": 500, "payload": ""})";
    
    Message msg = MessageParser::parse(jsonStr);

    EXPECT_EQ(msg.type, MessageType::Bet);
    EXPECT_EQ(msg.playerId, 42);
    EXPECT_EQ(msg.tableId, 1);
    EXPECT_EQ(msg.amount, 500);
    EXPECT_EQ(msg.payload, "");
}

TEST(MessageParserTest, SerializesMessageToJson) {
    Message msg;
    msg.type = MessageType::JoinTable; // Индекс 1
    msg.playerId = 99;
    msg.tableId = 5;
    msg.amount = 0;
    msg.payload = "Joining";

    std::string jsonStr = MessageParser::serialize(msg);

    EXPECT_NE(jsonStr.find("\"type\":\"join\""), std::string::npos);
    EXPECT_NE(jsonStr.find("\"playerId\":99"), std::string::npos);
    EXPECT_NE(jsonStr.find("\"tableId\":5"), std::string::npos);
    EXPECT_NE(jsonStr.find("\"payload\":\"Joining\""), std::string::npos);
}