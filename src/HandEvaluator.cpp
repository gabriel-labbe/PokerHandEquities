#include "../include/HandEvaluator.h"

HandValue HandEvaluator::evaluate(const Board& board, const std::vector<Card>& hand) {
    std::array<int, 15> rankCount = board.getRankCount();
    std::array<int, 4> suitCount = board.getSuitCount();
    uint16_t rankMask = board.getRankMask();

    // Add hand cards to counts in single pass
    for (const auto& c : hand) {
        int r = static_cast<int>(c.getRank());
        rankCount[r]++;
        suitCount[static_cast<int>(c.getSuit())]++;
        rankMask |= (1 << r);
    }

    // Check for flush in single pass
    int flushSuit = -1;
    if (board.isFlushPossible()) {
        for (int s = 0; s < 4; ++s) {
            if (suitCount[s] >= 5) {
                flushSuit = s;
                break;
            }
        }
    }

    // If flush exists, check for straight flush first
    if (flushSuit != -1) {
        uint16_t flushMask = 0;
        // Build flush mask in single pass through all cards
        for (const auto& c : board.getCards()) {
            if (static_cast<int>(c.getSuit()) == flushSuit)
                flushMask |= (1 << static_cast<int>(c.getRank()));
        }
        for (const auto& c : hand) {
            if (static_cast<int>(c.getSuit()) == flushSuit)
                flushMask |= (1 << static_cast<int>(c.getRank()));
        }

        int topStraight = getTopStraightRank(flushMask);
        if (topStraight != 0) {
            return {9, {static_cast<Card::Rank>(topStraight)}};
        }
    }

    // Single pass to find all rank patterns
    int fourKind = 0, threeKind = 0, pairs[2] = {0, 0};
    int pairCount = 0;
    
    for (int r = 14; r >= 2; --r) {
        if (rankCount[r] == 4) {
            fourKind = r;
            break; // Four of a kind found, no need to continue
        } else if (rankCount[r] == 3) {
            if (threeKind == 0) {
                threeKind = r;
            } else if (pairCount < 2) {
                // Second three-of-a-kind counts as a pair for full house
                pairs[pairCount++] = r;
            }
        } else if (rankCount[r] == 2) {
            if (pairCount < 2) pairs[pairCount++] = r;
        }
    }

    // Four of a kind
    if (fourKind) {
        Card::Rank kicker = getHighestKicker(rankCount, fourKind);
        return {8, {static_cast<Card::Rank>(fourKind), kicker}};
    }

    // Full house - simplified without extra loop
    if (threeKind && pairCount > 0) {
        return {7, {static_cast<Card::Rank>(threeKind), static_cast<Card::Rank>(pairs[0])}};
    }

    // Regular flush
    if (flushSuit != -1) {
        // Optimized flush rank collection
        Card::Rank flushRanks[5];
        int flushCount = 0;
        
        for (int r = 14; r >= 2 && flushCount < 5; --r) {
            // Check if this rank exists in the flush suit
            bool hasRank = false;
            for (const auto& c : board.getCards()) {
                if (static_cast<int>(c.getSuit()) == flushSuit && static_cast<int>(c.getRank()) == r) {
                    hasRank = true;
                    break;
                }
            }
            if (!hasRank) {
                for (const auto& c : hand) {
                    if (static_cast<int>(c.getSuit()) == flushSuit && static_cast<int>(c.getRank()) == r) {
                        hasRank = true;
                        break;
                    }
                }
            }
            if (hasRank) {
                flushRanks[flushCount++] = static_cast<Card::Rank>(r);
            }
        }
        
        return {6, std::vector<Card::Rank>(flushRanks, flushRanks + flushCount)};
    }

    // Straight
    int topStraight = getTopStraightRank(rankMask);
    if (topStraight != 0) {
        return {5, {static_cast<Card::Rank>(topStraight)}};
    }

    // Three of a kind
    if (threeKind) {
        auto kickers = getTopKickers(rankCount, {threeKind}, 2);
        return {4, {static_cast<Card::Rank>(threeKind), kickers[0], kickers[1]}};
    }

    // Two pair
    if (pairCount >= 2) {
        auto kicker = getTopKickers(rankCount, {pairs[0], pairs[1]}, 1);
        return {3, {static_cast<Card::Rank>(pairs[0]), static_cast<Card::Rank>(pairs[1]), kicker[0]}};
    }

    // One pair
    if (pairCount == 1) {
        auto kickers = getTopKickers(rankCount, {pairs[0]}, 3);
        return {2, {static_cast<Card::Rank>(pairs[0]), kickers[0], kickers[1], kickers[2]}};
    }

    // High card
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
            return static_cast<Card::Rank>(r);
    }
    return Card::Rank::Two;
}

std::vector<Card::Rank> HandEvaluator::getTopKickers(const std::array<int, 15>& count, const std::vector<int>& exclude, int needed) {
    std::vector<Card::Rank> result;
    result.reserve(needed); // Pre-allocate to avoid reallocations
    for (int r = 14; r >= 2 && static_cast<int>(result.size()) < needed; --r) {
        if (std::find(exclude.begin(), exclude.end(), r) == exclude.end() && count[r] > 0)
            result.push_back(static_cast<Card::Rank>(r));
    }
    return result;
}
