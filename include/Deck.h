#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include "Hand.h"
#include <vector>

class Deck {
public:
    Deck();

    void shuffle();
    std::vector<Card> getCards() const;
    
    // Generate all possible 2-card hand combinations
    static std::vector<Hand> generateAllHandCombinations();

private:
    std::vector<Card> cards;
    
    // Helper method to create a full deck
    static std::vector<Card> createFullDeck();
};

#endif
