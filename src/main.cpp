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
    double mcEquity = EquityCalculator::calculateEquity(hand1, hand2, iterations);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> mcElapsed = end - start;

    std::cout << "   Equity: " << mcEquity << "%" << std::endl;
    std::cout << "   Time taken: " << mcElapsed.count() << " seconds" << std::endl;
    std::cout << std::endl;

    // Test exact calculation method
    std::cout << "2. Exact Calculation Method (all possible boards):" << std::endl;
    start = std::chrono::steady_clock::now();
    double exactEquity = EquityCalculator::calculateExactEquity(hand1, hand2);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> exactElapsed = end - start;

    std::cout << "   Exact equity: " << exactEquity << "%" << std::endl;
    std::cout << "   Time taken: " << exactElapsed.count() << " seconds" << std::endl;
    std::cout << std::endl;

    // Compare results
    std::cout << "=== Comparison ===" << std::endl;
    std::cout << "Monte Carlo equity: " << mcEquity << "%" << std::endl;
    std::cout << "Exact equity:       " << exactEquity << "%" << std::endl;
    std::cout << "Difference:         " << std::abs(mcEquity - exactEquity) << "%" << std::endl;
    std::cout << "Speed ratio:        " << (exactElapsed.count() / mcElapsed.count()) << "x slower for exact method" << std::endl;

    return 0;
}
