#define BOOST_TEST_MODULE RoundManagerTests
#include <boost/test/included/unit_test.hpp>

#include "../include/game/RoundManager.hpp"

using namespace poker;

static std::shared_ptr<Player> makePlayer(PlayerId id, int chips = 1000)
{
    auto p = std::make_shared<Player>(id, "P" + std::to_string(id));
    p->setChips(chips);
    p->setState(PlayerState::Active);
    return p;
}

BOOST_AUTO_TEST_CASE(betting_round_and_turn_order)
{
    RoundManager rm;
    std::vector<std::shared_ptr<Player>> players = {
        makePlayer(1),
        makePlayer(2),
        makePlayer(3)
    };
    rm.setPlayers(players);

    auto first = rm.startBettingRound(0); // после дилера 0 будет игрок 2
    BOOST_REQUIRE(first);
    BOOST_CHECK_EQUAL(first->getId(), 2u);

    // Игрок 2 делает бет 100
    BOOST_CHECK(rm.bet(2, 100));
    BOOST_CHECK_EQUAL(rm.getPot(), 100);
    BOOST_CHECK_EQUAL(rm.getCurrentBet(), 100);

    // Следующим должен быть игрок 3
    auto next = rm.nextActiveAfter(2);
    BOOST_REQUIRE(next);
    BOOST_CHECK_EQUAL(next->getId(), 3u);

    // Игрок 3 коллирует
    BOOST_CHECK(rm.call(3));
    BOOST_CHECK_EQUAL(rm.getPot(), 200);

    // Затем очередь возвращается к игроку 1
    auto next2 = rm.nextActiveAfter(3);
    BOOST_REQUIRE(next2);
    BOOST_CHECK_EQUAL(next2->getId(), 1u);
}

BOOST_AUTO_TEST_CASE(fold_and_last_active_player)
{
    RoundManager rm;
    std::vector<std::shared_ptr<Player>> players = {
        makePlayer(1),
        makePlayer(2),
        makePlayer(3)
    };
    rm.setPlayers(players);

    rm.fold(1);
    rm.fold(2);

    BOOST_CHECK_EQUAL(rm.countActivePlayers(), 1u);
    auto last = rm.lastActivePlayer();
    BOOST_REQUIRE(last);
    BOOST_CHECK_EQUAL(last->getId(), 3u);
}

