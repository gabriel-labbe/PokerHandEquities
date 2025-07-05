#include <iostream>
#include <cstdint>
#include <vector>
#include <array>
#include <chrono>
#include "../include/Card.h"
#include "../include/Deck.h"
#include "../include/Hand.h"
#include "../include/Board.h"
#include "../include/HandEvaluator.h"

// Structure to store equity data for each hand pair
struct EquityData {
    int wins = 0;
    int ties = 0;
    int total = 0;
    
    double getEquity() const {
        if (total == 0) return 0.0;
        return (wins + 0.5 * ties) / total;
    }
};

int main() {
    // Generate all possible 2-card hands
    std::vector<Hand> allHands = Deck::generateAllHandCombinations();
    
    // Create data structure to store equity data
    const int numHands = allHands.size();
    std::vector<std::vector<EquityData>> equityTable(numHands, std::vector<EquityData>(numHands));
    
    // Get full deck
    Deck deck;
    deck.shuffle();
    std::vector<Card> fullDeck = deck.getCards();
    
    // Start timing
    auto start = std::chrono::steady_clock::now();
    int boardCount = 0;
    bool timeUp = false;
    
    for (size_t i1 = 0; i1 < fullDeck.size() && !timeUp; ++i1) {
        for (size_t i2 = i1 + 1; i2 < fullDeck.size() && !timeUp; ++i2) {
            for (size_t i3 = i2 + 1; i3 < fullDeck.size() && !timeUp; ++i3) {
                for (size_t i4 = i3 + 1; i4 < fullDeck.size() && !timeUp; ++i4) {
                    for (size_t i5 = i4 + 1; i5 < fullDeck.size() && !timeUp; ++i5) {
                        std::vector<Card> boardCards = {
                            fullDeck[i1], fullDeck[i2], fullDeck[i3], fullDeck[i4], fullDeck[i5]
                        };
                        
                        Board board(boardCards);
                        uint64_t boardMask = 0;
                        for (const auto& card : boardCards) {
                            boardMask |= (1ULL << card.getId());
                        }
                        
                        // Find all hands that don't overlap with the board
                        std::vector<int> validHandIndices;
                        validHandIndices.reserve(1081);
                        for (int i = 0; i < numHands; i++) {
                            if ((allHands[i].getMask() & boardMask) == 0) {
                                validHandIndices.push_back(i);
                            }
                        }
                        
                        // Evaluate hand strengths for all valid hands
                        std::vector<HandValue> handStrengths(validHandIndices.size());
                        for (size_t i = 0; i < validHandIndices.size(); i++) {
                            int handIdx = validHandIndices[i];
                            const Hand& hand = allHands[handIdx];
                            handStrengths[i] = HandEvaluator::evaluate(board, hand);
                        }
                        
                        // Compare all pairs of valid hands and update equity table
                        for (size_t i = 0; i < validHandIndices.size(); i++) {
                            for (size_t j = i + 1; j < validHandIndices.size(); j++) {
                                int hand1Idx = validHandIndices[i];
                                int hand2Idx = validHandIndices[j];
                                
                                // Check if hands don't share cards
                                if ((allHands[hand1Idx].getMask() & allHands[hand2Idx].getMask()) == 0) {
                                    const HandValue& strength1 = handStrengths[i];
                                    const HandValue& strength2 = handStrengths[j];
                                    
                                    // Update equity data
                                    equityTable[hand1Idx][hand2Idx].total++;
                                    equityTable[hand2Idx][hand1Idx].total++;
                                    
                                    if (strength1 > strength2) {
                                        equityTable[hand1Idx][hand2Idx].wins++;
                                    } else if (strength2 > strength1) {
                                        equityTable[hand2Idx][hand1Idx].wins++;
                                    } else {
                                        equityTable[hand1Idx][hand2Idx].ties++;
                                        equityTable[hand2Idx][hand1Idx].ties++;
                                    }
                                }
                            }
                        }
                        
                        boardCount++;
                        
                        // Check time
                        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() > 10) {
                            timeUp = true;
                            break;
                        }
                    }
                    if (timeUp) break;
                }
                if (timeUp) break;
            }
            if (timeUp) break;
        }
        if (timeUp) break;
    }
    
    std::cout << "Processed " << boardCount << " boards in approximately 10 seconds." << std::endl;

    return 0;
}
