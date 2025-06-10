#include <iostream>
#include <cstdint>
#include "../include/Card.h"
#include "../include/Deck.h"

uint64_t getMask(const std::vector<Card>& cards) {
    uint64_t mask = 0;
    for (const auto& card : cards) {
        mask |= (1ULL << card.getId());
    }
    return mask;
}

int main() {
    std::cout << "=== Bitmask Overlap Test ===" << std::endl;

    // Example cards
    Card Ah(Card::Rank::Ace, Card::Suit::Hearts);
    Card Kh(Card::Rank::King, Card::Suit::Hearts);
    Card Ad(Card::Rank::Ace, Card::Suit::Diamonds);
    Card Qd(Card::Rank::Queen, Card::Suit::Diamonds);
    Card Kd(Card::Rank::King, Card::Suit::Diamonds);

    // Hand1: Ah Kh
    std::vector<Card> hand1 = {Ah, Kh};
    uint64_t mask1 = getMask(hand1);

    // Hand2: Ah Qd (overlaps with hand1 on Ah)
    std::vector<Card> hand2 = {Ah, Qd};
    uint64_t mask2 = getMask(hand2);

    // Hand3: Ad Kd (no overlap with hand1)
    std::vector<Card> hand3 = {Ad, Kd};
    uint64_t mask3 = getMask(hand3);

    // Sample board with Ah and 2c
    Card twoC(Card::Rank::Two, Card::Suit::Clubs);
    std::vector<Card> board = {Ah, twoC};
    uint64_t board_mask = getMask(board);

    // Check hand-hand overlaps
    std::cout << "Hand1 (Ah Kh) and Hand2 (Ah Qd) overlap: " << ((mask1 & mask2) != 0 ? "Yes" : "No") << std::endl;
    std::cout << "Hand1 (Ah Kh) and Hand3 (Ad Kd) overlap: " << ((mask1 & mask3) != 0 ? "Yes" : "No") << std::endl;

    // Check hand-board overlaps
    std::cout << "Hand1 (Ah Kh) and Board (containing Ah) overlap: " << ((mask1 & board_mask) != 0 ? "Yes" : "No") << std::endl;
    std::cout << "Hand3 (Ad Kd) and Board (containing Ah) overlap: " << ((mask3 & board_mask) != 0 ? "Yes" : "No") << std::endl;

    // To integrate with all hands, e.g., filter for a "board"
    auto allHands = Deck::generateAllHandCombinations();
    std::cout << "Total hands: " << allHands.size() << std::endl;

    // Example: count how many hands overlap with board_mask
    int overlapping = 0;
    for (const auto& hand : allHands) {
        uint64_t hmask = getMask(hand);
        if (hmask & board_mask) overlapping++;
    }
    std::cout << "Hands overlapping with board: " << overlapping << std::endl;

    return 0;
}
