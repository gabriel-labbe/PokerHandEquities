#include <iostream>
#include "../include/Card.h"
#include "../include/Board.h"
#include "../include/HandEvaluator.h"
#include <cassert>

void testEvaluateStraightFlush() {
    std::vector<Card> board = {
        Card(Card::Rank::Ten, Card::Suit::Hearts),
        Card(Card::Rank::Jack, Card::Suit::Hearts),
        Card(Card::Rank::Queen, Card::Suit::Hearts),
        Card(Card::Rank::King, Card::Suit::Hearts),
        Card(Card::Rank::Two, Card::Suit::Spades)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Ace, Card::Suit::Hearts),
        Card(Card::Rank::Five, Card::Suit::Diamonds)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    std::cout << value.rankCategory << std::endl;

    assert(value.rankCategory == 9);
    assert(value.kickers[0] == Card::Rank::Ace);
    std::cout << "Test évaluer quinte flush réussi." << std::endl;
}

void testEvaluateFourOfAKind() {
    std::vector<Card> board = {
        Card(Card::Rank::Nine, Card::Suit::Diamonds),
        Card(Card::Rank::Nine, Card::Suit::Hearts),
        Card(Card::Rank::Nine, Card::Suit::Clubs),
        Card(Card::Rank::Eight, Card::Suit::Spades)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Nine, Card::Suit::Spades),
        Card(Card::Rank::Two, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 8);
    assert(value.kickers[0] == Card::Rank::Nine);
    assert(value.kickers[1] == Card::Rank::Eight);
    std::cout << "Test évaluer carré réussi." << std::endl;
}

void testEvaluateOnePair() {
    std::vector<Card> board = {
        Card(Card::Rank::Four, Card::Suit::Diamonds),
        Card(Card::Rank::Five, Card::Suit::Hearts),
        Card(Card::Rank::Seven, Card::Suit::Clubs)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Four, Card::Suit::Spades),
        Card(Card::Rank::Nine, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    std::cout << value.rankCategory << std::endl;
    assert(value.rankCategory == 2);
    assert(value.kickers[0] == Card::Rank::Four);
    assert(value.kickers[1] == Card::Rank::Nine);
    std::cout << "Test évaluer une paire réussi." << std::endl;
}


int main() {
    testEvaluateStraightFlush();
    testEvaluateFourOfAKind();
    testEvaluateOnePair();
    return 0;
}
