#include <iostream>
#include <cstdint>
#include <vector>
#include <array>
#include "../include/Card.h"
#include "../include/Deck.h"
#include "../include/Board.h"
#include "../include/HandEvaluator.h"

// Structure to represent a 2-card hand
struct Hand {
    Card card1, card2;
    uint64_t mask;
    
    Hand(Card c1, Card c2) : card1(c1), card2(c2) {
        mask = (1ULL << c1.getId()) | (1ULL << c2.getId());
    }
};

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
    std::cout << "Generating preflop equity lookup table..." << std::endl;
    
    // Step 1: Generate all possible 2-card hands
    std::vector<Hand> allHands;
    std::cout << "Step 1: Generating all hand combinations..." << std::endl;
    
    for (int suit1 = 0; suit1 < 4; suit1++) {
        for (int rank1 = 0; rank1 < 13; rank1++) {
            for (int suit2 = 0; suit2 < 4; suit2++) {
                for (int rank2 = 0; rank2 < 13; rank2++) {
                    // Avoid duplicate hands (ensure card1 < card2 by ID)
                    int id1 = suit1 * 13 + rank1;
                    int id2 = suit2 * 13 + rank2;
                    if (id1 < id2) {
                        Card c1(static_cast<Card::Rank>(rank1 + 2), static_cast<Card::Suit>(suit1));
                        Card c2(static_cast<Card::Rank>(rank2 + 2), static_cast<Card::Suit>(suit2));
                        allHands.emplace_back(c1, c2);
                    }
                }
            }
        }
    }
    
    std::cout << "Generated " << allHands.size() << " unique hand combinations" << std::endl;
    
    // Step 2: Create data structure to store equity data
    std::cout << "Step 2: Creating equity data structure..." << std::endl;
    const int numHands = allHands.size();
    std::vector<std::vector<EquityData>> equityTable(numHands, std::vector<EquityData>(numHands));
    
    // Step 3: Generate a single board for testing
    std::cout << "Step 3: Generating a single test board..." << std::endl;
    std::vector<Card> boardCards = {
        Card(Card::Rank::Ace, Card::Suit::Hearts),
        Card(Card::Rank::King, Card::Suit::Diamonds), 
        Card(Card::Rank::Queen, Card::Suit::Clubs),
        Card(Card::Rank::Jack, Card::Suit::Spades),
        Card(Card::Rank::Ten, Card::Suit::Hearts)
    };
    
    Board board(boardCards);
    uint64_t boardMask = 0;
    for (const auto& card : boardCards) {
        boardMask |= (1ULL << card.getId());
    }
    
    std::cout << "Board: ";
    for (const auto& card : boardCards) {
        std::cout << card << " ";
    }
    std::cout << std::endl;
    
    // Step 4: Evaluate valid scenarios for this board
    std::cout << "Step 4: Evaluating valid scenarios for the board..." << std::endl;
    
    // Find all hands that don't overlap with the board
    std::vector<int> validHandIndices;
    for (int i = 0; i < numHands; i++) {
        if ((allHands[i].mask & boardMask) == 0) {
            validHandIndices.push_back(i);
        }
    }
    
    std::cout << "Found " << validHandIndices.size() << " hands that don't overlap with board" << std::endl;
    
    // Evaluate hand strengths for all valid hands
    std::vector<HandValue> handStrengths(validHandIndices.size());
    for (size_t i = 0; i < validHandIndices.size(); i++) {
        int handIdx = validHandIndices[i];
        const Hand& hand = allHands[handIdx];
        std::vector<Card> handCards = {hand.card1, hand.card2};
        handStrengths[i] = HandEvaluator::evaluate(board, handCards);
    }
    
    // Compare all pairs of valid hands and update equity table
    int comparisons = 0;
    for (size_t i = 0; i < validHandIndices.size(); i++) {
        for (size_t j = i + 1; j < validHandIndices.size(); j++) {
            int hand1Idx = validHandIndices[i];
            int hand2Idx = validHandIndices[j];
            
            // Check if hands don't share cards
            if ((allHands[hand1Idx].mask & allHands[hand2Idx].mask) == 0) {
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
                
                comparisons++;
            }
        }
    }
    
    std::cout << "Processed " << comparisons << " valid hand pair comparisons" << std::endl;
    
    // Display some sample results
    std::cout << "\nSample equity results for first few valid hand pairs:" << std::endl;
    int sampleCount = 0;
    for (size_t i = 0; i < validHandIndices.size() && sampleCount < 5; i++) {
        for (size_t j = i + 1; j < validHandIndices.size() && sampleCount < 5; j++) {
            int hand1Idx = validHandIndices[i];
            int hand2Idx = validHandIndices[j];
            
            if ((allHands[hand1Idx].mask & allHands[hand2Idx].mask) == 0) {
                const Hand& hand1 = allHands[hand1Idx];
                const Hand& hand2 = allHands[hand2Idx];
                const EquityData& equity = equityTable[hand1Idx][hand2Idx];
                
                std::cout << hand1.card1 << hand1.card2 << " vs " << hand2.card1 << hand2.card2 
                         << ": " << equity.wins << "W-" << equity.ties << "T-" << equity.total << "T "
                         << "(Equity: " << equity.getEquity() << ")" << std::endl;
                sampleCount++;
            }
        }
    }

    return 0;
}
