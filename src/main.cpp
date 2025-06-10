#include <iostream>
#include <cstdint>
#include <vector>
#include <array>
#include <functional>
#include "../include/Card.h"
#include "../include/Deck.h"
#include "../include/Board.h"
#include "../include/HandEvaluator.h"

uint64_t getMask(const std::vector<Card>& cards) {
    uint64_t mask = 0;
    for (const auto& card : cards) {
        mask |= (1ULL << card.getId());
    }
    return mask;
}

int main() {
    // Generate all hand combinations
    std::vector<std::vector<Card>> all_hands = Deck::generateAllHandCombinations();
    int num_hands = all_hands.size();  // Should be 1326

    // Data structure: data[i][j][0] = wins for i vs j, [1] = ties, [2] = total
    std::vector<std::vector<std::array<long long, 3>>> data(
        num_hands, std::vector<std::array<long long, 3>>(num_hands, {0, 0, 0}));

    // Generate a single board, e.g., first 5 cards
    std::vector<Card> full_deck = Deck().getCards();  // Assuming Deck creates full deck
    std::vector<Card> board_cards(full_deck.begin(), full_deck.begin() + 5);
    Board board(board_cards);
    uint64_t board_mask = getMask(board_cards);

    // Find valid hands
    std::vector<int> valid_indices;
    std::vector<uint64_t> hand_masks(num_hands);
    for (int i = 0; i < num_hands; ++i) {
        hand_masks[i] = getMask(all_hands[i]);
        if ((hand_masks[i] & board_mask) == 0) {
            valid_indices.push_back(i);
        }
    }

    // Compute hand values
    std::vector<HandValue> hand_values;
    for (int idx : valid_indices) {
        hand_values.push_back(HandEvaluator::evaluate(board, all_hands[idx]));
    }

    // Update for pairs
    int num_valid = valid_indices.size();
    for (int a = 0; a < num_valid; ++a) {
        for (int b = a + 1; b < num_valid; ++b) {
            int h1 = valid_indices[a], h2 = valid_indices[b];
            if ((hand_masks[h1] & hand_masks[h2]) != 0) continue;

            const HandValue& v1 = hand_values[a];
            const HandValue& v2 = hand_values[b];

            data[h1][h2][2]++;
            data[h2][h1][2]++;

            if (v1 > v2) {
                data[h1][h2][0]++;
                data[h2][h1][1]++;
            } else if (v2 > v1) {
                data[h1][h2][1]++;
                data[h2][h1][0]++;
            } else {
                data[h1][h2][1]++;
                data[h2][h1][1]++;
            }
        }
    }

    // For demonstration, print something
    std::cout << "Processed one board with " << valid_indices.size() << " valid hands." << std::endl;

    return 0;
}
