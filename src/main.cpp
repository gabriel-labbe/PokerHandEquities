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
    int iterations = 1000000;

    auto start = std::chrono::steady_clock::now();
    double equity = EquityCalculator::calculateEquity(hand1, hand2, iterations);
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Equity of AhKh vs Ts9s: " << equity << "%" << std::endl;
    std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;

    auto start2 = std::chrono::steady_clock::now();
    double equity2 = EquityCalculator::calculateExactEquity(hand1, hand2);
    auto end2 = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed2 = end2 - start2;

    std::cout << "Exact Equity of AhKh vs Ts9s: " << equity2 << "%" << std::endl;
    std::cout << "Time taken for exact: " << elapsed2.count() << " seconds" << std::endl;

    return 0;
}
