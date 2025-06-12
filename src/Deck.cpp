#include "../include/Deck.h"
#include <algorithm>
#include <random>
#include <chrono>

Deck::Deck() {
    cards = createFullDeck();
}

void Deck::shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

std::vector<Card> Deck::getCards() const {
    return cards;
}

std::vector<Card> Deck::createFullDeck() {
    std::vector<Card> fullDeck;
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = static_cast<int>(Card::Rank::Two); rank <= static_cast<int>(Card::Rank::Ace); ++rank) {
            fullDeck.emplace_back(
                static_cast<Card::Rank>(rank),
                static_cast<Card::Suit>(suit)
            );
        }
    }
    return fullDeck;
}

std::vector<Hand> Deck::generateAllHandCombinations() {
    std::vector<Hand> handCombinations;
    
    // Create a full deck using the helper method
    std::vector<Card> fullDeck = createFullDeck();
    
    // Generate all possible 2-card combinations
    for (size_t i = 0; i < fullDeck.size(); ++i) {
        for (size_t j = i + 1; j < fullDeck.size(); ++j) {
            handCombinations.emplace_back(fullDeck[i], fullDeck[j]);
        }
    }
    
    return handCombinations;
}
