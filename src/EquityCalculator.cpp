#include "../include/EquityCalculator.h"
#include "../include/Deck.h"
#include "../include/Board.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <vector>

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

double EquityCalculator::calculateEquity(const std::vector<std::vector<Card>>& group1, const std::vector<std::vector<Card>>& group2, int iterations) {
    Deck fullDeck;
    std::vector<Card> full_cards = fullDeck.getCards();

    // Assume groups are not empty
    if (group1.empty() || group2.empty()) {
        return 0.0; // Or handle error
    }

    // Determine the number of threads to use based on hardware concurrency
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 1; // Fallback in case hardware_concurrency fails

    int iters_per_thread = iterations / num_threads;
    int remainder = iterations % num_threads;

    // Vectors to hold win and tie counts for each thread
    std::vector<int> wins(num_threads, 0);
    std::vector<int> ties(num_threads, 0);

    // Vector to hold the threads
    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < num_threads; ++i) {
        int start = i * iters_per_thread;
        int end = start + iters_per_thread;
        if (i == num_threads - 1) end += remainder;

        // Launch a thread for each portion of the iterations
        threads.emplace_back([&, i, start, end]() {
            // Initialize a random number generator unique to this thread
            std::random_device rd;
            std::mt19937 g(rd());

            int local_win = 0, local_tie = 0;
            for (int j = start; j < end; ++j) {
                // Select random hands from groups
                size_t h1_idx = g() % group1.size();
                size_t h2_idx = g() % group2.size();
                const auto& selected_hand1 = group1[h1_idx];
                const auto& selected_hand2 = group2[h2_idx];

                // Create local remaining and remove selected cards
                std::vector<Card> local_remaining = full_cards;
                auto removeCard = [&](const Card& c) {
                    local_remaining.erase(std::remove_if(local_remaining.begin(), local_remaining.end(), [&](const Card& d) {
                        return d.getRank() == c.getRank() && d.getSuit() == c.getSuit();
                    }), local_remaining.end());
                };
                for (const auto& c : selected_hand1) removeCard(c);
                for (const auto& c : selected_hand2) removeCard(c);

                // Shuffle the local deck for this simulation
                std::shuffle(local_remaining.begin(), local_remaining.end(), g);

                // Select the first 5 cards as the board
                std::vector<Card> boardCards(local_remaining.begin(), local_remaining.begin() + 5);
                Board board(boardCards);

                // Evaluate both hands
                HandValue val1 = HandEvaluator::evaluate(board, selected_hand1);
                HandValue val2 = HandEvaluator::evaluate(board, selected_hand2);

                int cmp = compareHands(val1, val2);
                if (cmp > 0) {
                    local_win++;
                } else if (cmp == 0) {
                    local_tie++;
                }
            }

            // Store the results in the shared vectors (safe since each thread writes to its own index)
            wins[i] = local_win;
            ties[i] = local_tie;
        });
    }

    // Wait for all threads to complete their work
    for (auto& t : threads) {
        t.join();
    }

    // Aggregate the results from all threads
    int total_win1 = 0, total_tie = 0;
    for (int w : wins) total_win1 += w;
    for (int t : ties) total_tie += t;

    // Calculate the equity
    double equity = (total_win1 + total_tie * 0.5) / static_cast<double>(iterations) * 100.0;
    return equity;
}
