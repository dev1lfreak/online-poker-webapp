#define BOOST_TEST_MODULE PokerRulesTests
#include <boost/test/included/unit_test.hpp>

#include "../include/game/PokerRules.hpp"
#include "../include/game/Card.hpp"

using namespace poker;

static std::shared_ptr<Player> makePlayerWithCards(PlayerId id, const std::vector<Card>& hand, int chips = 1000)
{
    auto p = std::make_shared<Player>(id, "P" + std::to_string(id));
    p->setChips(chips);
    p->setState(PlayerState::Active);
    for (auto c : hand) {
        p->giveCard(c);
    }
    return p;
}

BOOST_AUTO_TEST_CASE(determine_winner_simple_high_card)
{
    // Доска пуста, выигрывает тот, у кого старше карманка
    std::vector<Card> board;

    auto p1 = makePlayerWithCards(1, {
        {Rank::Ten, Suit::Hearts},
        {Rank::Nine, Suit::Clubs}
    });

    auto p2 = makePlayerWithCards(2, {
        {Rank::Ace, Suit::Spades},
        {Rank::Two, Suit::Diamonds}
    });

    std::vector<std::shared_ptr<Player>> players{p1, p2};

    auto winner = PokerRules::determineWinner(players, board);
    BOOST_REQUIRE(winner);
    BOOST_CHECK_EQUAL(winner->getId(), 2u);
}

