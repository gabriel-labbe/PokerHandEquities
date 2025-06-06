#include <iostream>
#include <chrono>
#include <iomanip>
#include "../include/Card.h"
#include "../include/EquityCalculator.h"

int main() {
std::vector<std::vector<Card>> group1 = {
    {
        Card(Card::Rank::Ace, Card::Suit::Hearts),
        Card(Card::Rank::King, Card::Suit::Hearts)
    },
    {
        Card(Card::Rank::Ace, Card::Suit::Spades),
        Card(Card::Rank::King, Card::Suit::Spades)
    },
    {
        Card(Card::Rank::Ace, Card::Suit::Diamonds),
        Card(Card::Rank::King, Card::Suit::Diamonds)
    }
};
std::vector<std::vector<Card>> group2 = {
    {
        Card(Card::Rank::Ten, Card::Suit::Spades),
        Card(Card::Rank::Nine, Card::Suit::Spades)
    },
    {
        Card(Card::Rank::Ten, Card::Suit::Clubs),
        Card(Card::Rank::Nine, Card::Suit::Clubs)
    },
    {
        Card(Card::Rank::Ten, Card::Suit::Diamonds),
        Card(Card::Rank::Nine, Card::Suit::Diamonds)
    }
};
int iterations = 100000;

    auto start = std::chrono::steady_clock::now();
double equity = EquityCalculator::calculateEquity(group1, group2, iterations);
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << std::fixed << std::setprecision(2);
std::cout << "Equity of group1 vs group2: " << equity << "%" << std::endl;
    std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
