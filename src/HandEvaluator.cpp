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
        uint16_t flushMask = board.getSuitRankMask(flushSuit);
        for (const auto& c : hand) {
            if ((int)c.getSuit() == flushSuit)
                flushMask |= (1 << (int)c.getRank());
        }

        int topStraight = getTopStraightRank(flushMask);
        if (topStraight != 0) {
            return {9, {(Card::Rank)topStraight}};
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
        uint16_t flushMask = board.getSuitRankMask(flushSuit);
        for (const auto& c : hand) {
            if ((int)c.getSuit() == flushSuit)
                flushMask |= (1 << (int)c.getRank());
        }
        std::vector<Card::Rank> flushRanks;
        for (int r = 14; r >= 2 && flushRanks.size() < 5; --r) {
            if (flushMask & (1 << r))
                flushRanks.push_back((Card::Rank)r);
        }
        return {6, flushRanks};
    }

    int topStraight = getTopStraightRank(rankMask);
    if (topStraight != 0) {
        return {5, {(Card::Rank)topStraight}};
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

int HandEvaluator::getTopStraightRank(uint16_t mask) {
    static const std::array<int, 10> topRanks = {14, 13, 12, 11, 10, 9, 8, 7, 6, 5};
    static const uint16_t straightMasks[10] = {
        (1<<14)|(1<<13)|(1<<12)|(1<<11)|(1<<10),
        (1<<13)|(1<<12)|(1<<11)|(1<<10)|(1<<9),
        (1<<12)|(1<<11)|(1<<10)|(1<<9)|(1<<8),
        (1<<11)|(1<<10)|(1<<9)|(1<<8)|(1<<7),
        (1<<10)|(1<<9)|(1<<8)|(1<<7)|(1<<6),
        (1<<9)|(1<<8)|(1<<7)|(1<<6)|(1<<5),
        (1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4),
        (1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3),
        (1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2),
        (1<<14)|(1<<5)|(1<<4)|(1<<3)|(1<<2)
    };
    for (int i = 0; i < 10; ++i) {
        if ((mask & straightMasks[i]) == straightMasks[i])
            return topRanks[i];
    }
    return 0;
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
