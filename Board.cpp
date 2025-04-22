#include "Board.h"

Board::Board(const std::vector<Card>& cards) : cards(cards) {
    computeCounters();
    computeFlushPossible();
    computePaired();
    computeStraightPossible();
}

void Board::computeCounters() {
    for (const auto& c : cards) {
        int r = static_cast<int>(c.getRank());
        rankCount[r]++;
        suitCount[static_cast<int>(c.getSuit())]++;
        rankMask |= (1 << r);
    }
    // Ace-low straight adjustment
    if (rankMask & (1 << 14))
        rankMask |= (1 << 1);
}

void Board::computeFlushPossible() {
    for (int s = 0; s < 4; ++s) {
        if (suitCount[s] >= 3) { // 3 because with 2 hole cards we might hit flush
            flushPossible = true;
            break;
        }
    }
}

void Board::computePaired() {
    for (int r = 2; r <= 14; ++r) {
        if (rankCount[r] >= 2) {
            paired = true;
            break;
        }
    }
}

// We could make bigger window jumps in some cases to speed things up
void Board::computeStraightPossible() {
    uint16_t mask = rankMask;
    for (int i = 0; i <= 9; ++i) {
        uint16_t window = (mask >> (10 - i)) & 0b11111;
        int count = 0;
        for (int b = 0; b < 5; ++b) {
            if (window & (1 << b)) count++;
        }
        if (count >= 3) {
            straightPossible = true;
            return;
        }
    }
    straightPossible = false;
}


const std::vector<Card>& Board::getCards() const { return cards; }
const std::array<int, 15>& Board::getRankCount() const { return rankCount; }
const std::array<int, 4>& Board::getSuitCount() const { return suitCount; }
uint16_t Board::getRankMask() const { return rankMask; }
bool Board::isFlushPossible() const { return flushPossible; }
bool Board::isPaired() const { return paired; }
bool Board::isStraightPossible() const { return straightPossible; }
