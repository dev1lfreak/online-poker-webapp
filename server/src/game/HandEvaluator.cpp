#include "../../include/game/HandEvaluator.hpp"
#include <algorithm>
#include <map>
#include <set>

namespace poker {
    bool HandScore::operator>(const HandScore &other) const {
        if (rank != other.rank)
            return rank > other.rank;
        return tiebreak > other.tiebreak;
    }

    static std::vector<int> ranks(const std::vector<Card> &cards) {
        std::vector<int> r;
        for (auto &c: cards)
            r.push_back((int) c.rank);

        std::sort(r.begin(), r.end(), std::greater<>());
        return r;
    }

    static bool isFlush(const std::vector<Card> &cards) {
        std::map<Suit, int> s;
        for (auto &c: cards)
            s[c.suit]++;

        for (auto [_,v]: s)
            if (v >= 5)
                return true;

        return false;
    }

    static bool isStraight(std::vector<int> r) {
        std::set<int> s(r.begin(), r.end());
        std::vector<int> u(s.begin(), s.end());

        if (u.size() < 5) return false;
        for (size_t i = 0; i + 4 < u.size(); i++)
            if (u[i] + 1 == u[i + 1] &&
                u[i] + 2 == u[i + 2] &&
                u[i] + 3 == u[i + 3] &&
                u[i] + 4 == u[i + 4])
                return true;

        return false;
    }

    HandScore HandEvaluator::evaluate(const std::vector<Card> &cards) {
        auto r = ranks(cards);

        std::map<int, int> freq;
        for (int v: r)
            freq[v]++;

        bool flush = isFlush(cards);
        bool straight = isStraight(r);

        if (straight && flush) {
            if (r[0] == 14)
                return {9, r}; // Royal Flush

            return {8, r}; // Straight Flush
        }

        bool four = false, three = false;
        int pairs = 0;

        for (auto [k,v]: freq) {
            if (v == 4) four = true;
            if (v == 3) three = true;
            if (v == 2) pairs++;
        }

        if (four) return {7, r}; // Four of a kind
        if (three && pairs >= 1) return {6, r}; // FullHouse
        if (flush) return {5, r}; //  Flash
        if (straight) return {4, r}; // Straight
        if (three) return {3, r}; // Three of a kind
        if (pairs >= 2) return {2, r}; // Two pairs
        if (pairs == 1) return {1, r}; // One pair

        return {0, r}; // High card
    }
}
