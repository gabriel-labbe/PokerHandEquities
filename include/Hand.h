#ifndef HAND_H
#define HAND_H

#include "Card.h"
#include <cstdint>
#include <vector>

class Hand {
public:
    Hand(Card c1, Card c2);

    Card getCard1() const;
    Card getCard2() const;
    uint64_t getMask() const;
    std::vector<Card> getCards() const;

private:
    Card card1, card2;
    uint64_t mask;
};

#endif
