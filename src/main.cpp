#include <iostream>
#include <chrono>
#include <iomanip>
#include "../include/Card.h"
#include "../include/EquityCalculator.h"

int main() {
    std::vector<Card> hand1 = {
        Card(Card::Rank::Ace, Card::Suit::Hearts),
        Card(Card::Rank::King, Card::Suit::Hearts)
    };
    std::vector<Card> hand2 = {
        Card(Card::Rank::Ten, Card::Suit::Spades),
        Card(Card::Rank::Nine, Card::Suit::Spades)
    };

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== Poker Hand Equity Calculator ===" << std::endl;
    std::cout << "Hand 1: AhKh" << std::endl;
    std::cout << "Hand 2: Ts9s" << std::endl;
    std::cout << std::endl;

    // Test Monte Carlo method
    std::cout << "1. Monte Carlo Method (1,000,000 iterations):" << std::endl;
    int iterations = 1000000;
    auto start = std::chrono::steady_clock::now();
    double mcEquity = EquityCalculator::calculateEquityMonteCarlo(hand1, hand2, iterations);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> mcElapsed = end - start;

    std::cout << "   Equity: " << mcEquity << "%" << std::endl;
    std::cout << "   Time taken: " << mcElapsed.count() << " seconds" << std::endl;
    std::cout << std::endl;

    // Test exact calculation method (loops)
    std::cout << "2. Exact Calculation Method (nested loops):" << std::endl;
    start = std::chrono::steady_clock::now();
    double exactEquity = EquityCalculator::calculateExactEquity(hand1, hand2);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> exactElapsed = end - start;

    std::cout << "   Exact equity: " << exactEquity << "%" << std::endl;
    std::cout << "   Time taken: " << exactElapsed.count() << " seconds" << std::endl;
    std::cout << std::endl;

    // Test recursive exact calculation
    std::cout << "3. Exact Calculation Method (recursive):" << std::endl;
    start = std::chrono::steady_clock::now();
    double recursiveEquity = EquityCalculator::calculateExactEquityRecursive(hand1, hand2);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> recursiveElapsed = end - start;

    std::cout << "   Exact equity: " << recursiveEquity << "%" << std::endl;
    std::cout << "   Time taken: " << recursiveElapsed.count() << " seconds" << std::endl;
    std::cout << std::endl;

    // Test permutation exact calculation
    std::cout << "4. Exact Calculation Method (next_permutation):" << std::endl;
    start = std::chrono::steady_clock::now();
    double permEquity = EquityCalculator::calculateExactEquityPermutation(hand1, hand2);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> permElapsed = end - start;

    std::cout << "   Exact equity: " << permEquity << "%" << std::endl;
    std::cout << "   Time taken: " << permElapsed.count() << " seconds" << std::endl;
    std::cout << std::endl;

    // Compare results
    std::cout << "=== Comparison of Exact Methods ===" << std::endl;
    std::cout << "Nested Loops Time: " << exactElapsed.count() << "s" << std::endl;
    std::cout << "Recursive Time:    " << recursiveElapsed.count() << "s" << std::endl;
    std::cout << "Permutation Time:  " << permElapsed.count() << "s" << std::endl;
    std::cout << std::endl;
    std::cout << "Monte Carlo equity: " << mcEquity << "%" << std::endl;
    std::cout << "Exact equity (loops): " << exactEquity << "%" << std::endl;
    std::cout << "Difference from Monte Carlo: " << std::abs(mcEquity - exactEquity) << "%" << std::endl;

    return 0;
}
