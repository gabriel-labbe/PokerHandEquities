#ifndef HAND_EVALUATOR_H
#define HAND_EVALUATOR_H

#include "Board.h"
#include "Card.h"
#include "Hand.h"
#include <vector>
#include <array>
#include <algorithm>

struct HandValue {
    int rankCategory;               // 9 = StraightFlush, 8 = FourOfAKind, ...
    std::vector<Card::Rank> kickers; // tie breakers in order

    bool operator>(const HandValue& other) const {
        if (rankCategory != other.rankCategory)
            return rankCategory > other.rankCategory;
        return kickers > other.kickers;
    }
};

class HandEvaluator {
public:
    static HandValue evaluate(const Board& board, const Hand& hand);
    static int getTopStraightRank(uint16_t mask);

private:
    static Card::Rank getHighestKicker(const std::array<int, 15>& count, int exclude);
    static std::vector<Card::Rank> getTopKickers(const std::array<int, 15>& count, const std::vector<int>& exclude, int needed);
};

#endif // HAND_EVALUATOR_H
