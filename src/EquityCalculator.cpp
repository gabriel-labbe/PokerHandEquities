#include "../include/EquityCalculator.h"
#include "../include/Deck.h"
#include "../include/Board.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

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

std::vector<Card> getRemainingDeck(const std::vector<Card>& hand1, const std::vector<Card>& hand2) {
    Deck fullDeck;
    std::vector<Card> remaining = fullDeck.getCards();

    auto removeCard = [&](const Card& c) {
        remaining.erase(std::remove_if(remaining.begin(), remaining.end(), [&](const Card& d) {
            return d.getRank() == c.getRank() && d.getSuit() == c.getSuit();
        }), remaining.end());
    };

    for (const auto& c : hand1) removeCard(c);
    for (const auto& c : hand2) removeCard(c);

    return remaining;
}

double EquityCalculator::calculateEquityMonteCarlo(const std::vector<Card>& hand1, const std::vector<Card>& hand2, int iterations) {
    std::vector<Card> remaining = getRemainingDeck(hand1, hand2);

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
            // Each thread works on a local copy of the remaining cards to avoid data races
            std::vector<Card> local_remaining = remaining;

            // Initialize a random number generator unique to this thread
            std::random_device rd;
            std::mt19937 g(rd());

            int local_win = 0, local_tie = 0;
            for (int j = start; j < end; ++j) {
                // Shuffle the local deck for this simulation
                std::shuffle(local_remaining.begin(), local_remaining.end(), g);

                // Select the first 5 cards as the board
                std::vector<Card> boardCards(local_remaining.begin(), local_remaining.begin() + 5);
                Board board(boardCards);

                // Evaluate both hands
                HandValue val1 = HandEvaluator::evaluate(board, hand1);
                HandValue val2 = HandEvaluator::evaluate(board, hand2);

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

double EquityCalculator::calculateExactEquity(const std::vector<Card>& hand1, const std::vector<Card>& hand2) {
    std::vector<Card> remaining = getRemainingDeck(hand1, hand2);

    long long totalBoards = 0;
    long long hand1Wins = 0;
    long long ties = 0;

    // Generate all possible 5-card combinations from remaining cards
    int n = remaining.size();
    
    for (int i = 0; i < n - 4; ++i) {
        for (int j = i + 1; j < n - 3; ++j) {
            for (int k = j + 1; k < n - 2; ++k) {
                for (int l = k + 1; l < n - 1; ++l) {
                    for (int m = l + 1; m < n; ++m) {
                        // Create board with these 5 cards
                        std::vector<Card> boardCards = {
                            remaining[i], remaining[j], remaining[k], remaining[l], remaining[m]
                        };
                        Board board(boardCards);

                        // Evaluate both hands on this board
                        HandValue val1 = HandEvaluator::evaluate(board, hand1);
                        HandValue val2 = HandEvaluator::evaluate(board, hand2);

                        // Compare hands
                        int cmp = compareHands(val1, val2);
                        if (cmp > 0) {
                            hand1Wins++;
                        } else if (cmp == 0) {
                            ties++;
                        }

                        totalBoards++;
                    }
                }
            }
        }
    }

    // Calculate exact equity
    double equity = (hand1Wins + ties * 0.5) / static_cast<double>(totalBoards) * 100.0;
    return equity;
}
