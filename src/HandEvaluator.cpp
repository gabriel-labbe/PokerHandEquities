#include "../include/HandEvaluator.h"

HandValue HandEvaluator::evaluate(const Board& board, const std::vector<Card>& hand) {
    std::array<int, 15> rankCount = board.getRankCount();
    std::array<int, 4> suitCount = board.getSuitCount();
    uint16_t rankMask = board.getRankMask();

    for (const auto& c : hand) {
        int r = (int)c.getRank();
        rankCount[r]++;
        suitCount[(int)c.getSuit()]++;
        rankMask |= (1 << r);
    }

    if (rankMask & (1 << 14))
        rankMask |= (1 << 1);

    int flushSuit = -1;
    if (board.isFlushPossible()) {
        for (int s = 0; s < 4; ++s) {
            if (suitCount[s] >= 5) {
                flushSuit = s;
                break;
            }
        }
    }

    if (flushSuit != -1) {
        uint16_t flushMask = 0;
        for (const auto& c : board.getCards()) {
            if ((int)c.getSuit() == flushSuit)
                flushMask |= (1 << (int)c.getRank());
        }
        for (const auto& c : hand) {
            if ((int)c.getSuit() == flushSuit)
                flushMask |= (1 << (int)c.getRank());
        }
        if (flushMask & (1 << 14))
            flushMask |= (1 << 1);

        if (isStraight(flushMask)) {
            return {9, {getTopStraightRank(flushMask)}};
        }
    }

    for (int r = 14; r >= 2; --r) {
        if (rankCount[r] == 4) {
            return {8, {(Card::Rank)r, getHighestKicker(rankCount, r)}};
        }
    }

    int three = 0, pair = 0;
    for (int r = 14; r >= 2; --r) {
        if (rankCount[r] >= 3 && three == 0) three = r;
        else if (rankCount[r] >= 2 && pair == 0) pair = r;
    }
    if (three && pair) {
        return {7, {(Card::Rank)three, (Card::Rank)pair}};
    }

    if (flushSuit != -1) {
        std::vector<Card::Rank> flushRanks;
        for (int r = 14; r >= 2 && flushRanks.size() < 5; --r) {
            for (const auto& c : board.getCards())
                if ((int)c.getSuit() == flushSuit && (int)c.getRank() == r)
                    flushRanks.push_back((Card::Rank)r);
            for (const auto& c : hand)
                if ((int)c.getSuit() == flushSuit && (int)c.getRank() == r)
                    flushRanks.push_back((Card::Rank)r);
        }
        return {6, flushRanks};
    }

    if (board.isStraightPossible() || isStraight(rankMask)) {
        return {5, {getTopStraightRank(rankMask)}};
    }

    three = 0;
    for (int r = 14; r >= 2; --r) {
        if (rankCount[r] == 3) {
            three = r;
            break;
        }
    }
    if (three) {
        auto kickers = getTopKickers(rankCount, {three}, 2);
        return {4, {(Card::Rank)three, kickers[0], kickers[1]}};
    }

    std::vector<int> pairs;
    for (int r = 14; r >= 2; --r) {
        if (rankCount[r] >= 2)
            pairs.push_back(r);
        if (pairs.size() == 2)
            break;
    }
    if (pairs.size() == 2) {
        auto kicker = getTopKickers(rankCount, {pairs[0], pairs[1]}, 1);
        return {3, {(Card::Rank)pairs[0], (Card::Rank)pairs[1], kicker[0]}};
    }

    if (pairs.size() == 1) {
        auto kickers = getTopKickers(rankCount, {pairs[0]}, 3);
        return {2, {(Card::Rank)pairs[0], kickers[0], kickers[1], kickers[2]}};
    }

    auto highCards = getTopKickers(rankCount, {}, 5);
    return {1, highCards};
}

bool HandEvaluator::isStraight(uint16_t mask) {
    static const uint16_t straightMasks[10] = {
        0b11111000000000, 0b01111100000000, 0b00111110000000,
        0b00011111000000, 0b00001111100000, 0b00000111110000,
        0b00000011111000, 0b00000001111100, 0b00000000111110,
        0b10000000011110
    };
    for (auto m : straightMasks) {
        if ((mask & m) == m)
            return true;
    }
    return false;
}

Card::Rank HandEvaluator::getTopStraightRank(uint16_t mask) {
    static const std::array<int, 10> topRanks = {14, 13, 12, 11, 10, 9, 8, 7, 6, 5};
    static const uint16_t straightMasks[10] = {
        0b11111000000000, 0b01111100000000, 0b00111110000000,
        0b00011111000000, 0b00001111100000, 0b00000111110000,
        0b00000011111000, 0b00000001111100, 0b00000000111110,
        0b10000000011110
    };
    for (int i = 0; i < 10; ++i) {
        if ((mask & straightMasks[i]) == straightMasks[i])
            return (Card::Rank)topRanks[i];
    }
    return Card::Rank::Two;
}

Card::Rank HandEvaluator::getHighestKicker(const std::array<int, 15>& count, int exclude) {
    for (int r = 14; r >= 2; --r) {
        if (r != exclude && count[r] > 0)
            return (Card::Rank)r;
    }
    return Card::Rank::Two;
}

std::vector<Card::Rank> HandEvaluator::getTopKickers(const std::array<int, 15>& count, const std::vector<int>& exclude, int needed) {
    std::vector<Card::Rank> result;
    for (int r = 14; r >= 2 && result.size() < needed; --r) {
        if (std::find(exclude.begin(), exclude.end(), r) == exclude.end() && count[r] > 0)
            result.push_back((Card::Rank)r);
    }
    return result;
}
