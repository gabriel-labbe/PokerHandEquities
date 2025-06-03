#include "../include/Deck.h"
#include <algorithm>
#include <random>
#include <chrono>

Deck::Deck() {
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = static_cast<int>(Card::Rank::Two); rank <= static_cast<int>(Card::Rank::Ace); ++rank) {
            cards.emplace_back(
                static_cast<Card::Rank>(rank),
                static_cast<Card::Suit>(suit)
            );
        }
    }
}

void Deck::shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

std::vector<Card> Deck::getCards() const {
    return cards;
}
