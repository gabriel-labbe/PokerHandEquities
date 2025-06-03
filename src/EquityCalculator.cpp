#include "../include/EquityCalculator.h"
#include "../include/Deck.h"
#include "../include/Board.h"
#include <algorithm>
#include <random>
#include <chrono>

int compareHands(const HandValue& a, const HandValue& b) {
    if (a.rankCategory > b.rankCategory) return 1;
    if (a.rankCategory < b.rankCategory) return -1;
    for (size_t i = 0; i < a.kickers.size(); ++i) {
        int ar = static_cast<int>(a.kickers[i]);
        int br = static_cast<int>(b.kickers[i]);
        if (ar > br) return 1;
        if (ar < br) return -1;
    }
    return 0;
}

double EquityCalculator::calculateEquity(const std::vector<Card>& hand1, const std::vector<Card>& hand2, int iterations) {
    Deck fullDeck;
    std::vector<Card> remaining = fullDeck.getCards();

    auto removeCard = [&](const Card& c) {
        remaining.erase(std::remove_if(remaining.begin(), remaining.end(), [&](const Card& d) {
            return d.getRank() == c.getRank() && d.getSuit() == c.getSuit();
        }), remaining.end());
    };

    for (const auto& c : hand1) removeCard(c);
    for (const auto& c : hand2) removeCard(c);

    int win1 = 0, tie = 0;
    for (int i = 0; i < iterations; ++i) {
        // Shuffle
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(remaining.begin(), remaining.end(), std::default_random_engine(seed));

        // Take first 5
        std::vector<Card> boardCards(remaining.begin(), remaining.begin() + 5);
        Board board(boardCards);

        HandValue val1 = HandEvaluator::evaluate(board, hand1);
        HandValue val2 = HandEvaluator::evaluate(board, hand2);

        int cmp = compareHands(val1, val2);
        if (cmp > 0) {
            win1++;
        } else if (cmp == 0) {
            tie++;
        }
    }

    double equity = (win1 + tie * 0.5) / static_cast<double>(iterations) * 100.0;
    return equity;
}
