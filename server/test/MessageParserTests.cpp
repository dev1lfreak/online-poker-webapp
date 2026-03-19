#define BOOST_TEST_MODULE MessageParserTests
#include <boost/test/included/unit_test.hpp>

#include "../include/protocol/MessageParser.hpp"

using namespace poker;

BOOST_AUTO_TEST_CASE(parse_bet_message_with_amount_and_ids)
{
    std::string json = R"({
        "type": "bet",
        "playerId": 5,
        "tableId": 2,
        "amount": 150
    })";

    Message msg = MessageParser::parse(json);

    BOOST_CHECK(msg.type == MessageType::Bet);
    BOOST_CHECK_EQUAL(msg.playerId, 5u);
    BOOST_CHECK_EQUAL(msg.tableId, 2);
    BOOST_CHECK_EQUAL(msg.amount, 150);
}

BOOST_AUTO_TEST_CASE(serialize_and_parse_roundtrip)
{
    Message original;
    original.type = MessageType::Call;
    original.playerId = 10;
    original.tableId = 3;
    original.amount = 200;
    original.payload = R"({"extra":"data"})";

    std::string json = MessageParser::serialize(original);
    Message parsed = MessageParser::parse(json);

    BOOST_CHECK(parsed.type == original.type);
    BOOST_CHECK_EQUAL(parsed.playerId, original.playerId);
    BOOST_CHECK_EQUAL(parsed.tableId, original.tableId);
    BOOST_CHECK_EQUAL(parsed.amount, original.amount);
}

