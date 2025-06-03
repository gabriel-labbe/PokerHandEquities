#include <iostream>
#include "../include/Card.h"
#include "../include/Deck.h"

int main() {
    Deck deck;
    const auto& cards = deck.getCards();
    for (const auto& card : cards) {
        std::cout << card << " ";
    }
    std::cout << std::endl;
    return 0;
}
