#include <iostream>
#include "../include/Card.h"
#include "../include/Deck.h"

int main() {
    std::cout << "=== Poker Hand Generation Test ===" << std::endl;
    
    // Test hand generation
    auto allHands = Deck::generateAllHandCombinations();
    std::cout << "Total possible 2-card hands: " << allHands.size() << std::endl;
    std::cout << "Expected: " << (52 * 51 / 2) << " combinations" << std::endl;
    std::cout << std::endl;
    
    std::cout << "First 10 hands:" << std::endl;
    for (size_t i = 0; i < std::min(size_t(10), allHands.size()); ++i) {
        std::cout << "Hand " << (i + 1) << ": ";
        for (size_t j = 0; j < allHands[i].size(); ++j) {
            std::cout << allHands[i][j].getString();
            if (j < allHands[i].size() - 1) std::cout << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
