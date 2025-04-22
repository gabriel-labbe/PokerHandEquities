#ifndef BOARD_H
#define BOARD_H

#include "Card.h"
#include <array>
#include <vector>
#include <cstdint>

class Board {
public:
    Board(const std::vector<Card>& cards);

    const std::vector<Card>& getCards() const;
    const std::array<int, 15>& getRankCount() const;
    const std::array<int, 4>& getSuitCount() const;
    uint16_t getRankMask() const;
    bool isFlushPossible() const;
    bool isPaired() const;
    bool isStraightPossible() const;

private:
    std::vector<Card> cards;
    std::array<int, 15> rankCount{};
    std::array<int, 4> suitCount{};
    uint16_t rankMask = 0;

    bool flushPossible = false;
    bool paired = false;
    bool straightPossible = false;

    void computeCounters();
    void computeFlushPossible();
    void computePaired();
    void computeStraightPossible();
};

#endif
