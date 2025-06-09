#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>

class Deck {
public:
    Deck();

    void shuffle();
    std::vector<Card> getCards() const;
    std::vector<std::vector<Card>> getAllHandCombinations() const;

private:
    std::vector<Card> cards;
};

#endif
