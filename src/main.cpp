#include <iostream>
#include "../include/Card.h"
#include "../include/Board.h"
#include "../include/HandEvaluator.h"
#include <cassert>

void testStraightFlush() {
    // Royal flush: A-K-Q-J-10 of hearts
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

    assert(value.rankCategory == 9);
    assert(value.kickers.size() == 1);
    assert(value.kickers[0] == Card::Rank::Ace);
    std::cout << "Straight Flush test passed." << std::endl;
}

void testFourOfAKind() {
    // Four 9s with 8 kicker
    std::vector<Card> board = {
        Card(Card::Rank::Nine, Card::Suit::Diamonds),
        Card(Card::Rank::Nine, Card::Suit::Hearts),
        Card(Card::Rank::Nine, Card::Suit::Clubs),
        Card(Card::Rank::Eight, Card::Suit::Spades),
        Card(Card::Rank::Three, Card::Suit::Hearts)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Nine, Card::Suit::Spades),
        Card(Card::Rank::Two, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 8);
    assert(value.kickers.size() == 2);
    assert(value.kickers[0] == Card::Rank::Nine);  // Quad rank
    assert(value.kickers[1] == Card::Rank::Eight); // Highest remaining card
    std::cout << "Four of a Kind test passed." << std::endl;
}

void testFullHouse() {
    // Three Kings, pair of 7s
    std::vector<Card> board = {
        Card(Card::Rank::King, Card::Suit::Hearts),
        Card(Card::Rank::King, Card::Suit::Diamonds),
        Card(Card::Rank::Seven, Card::Suit::Hearts),
        Card(Card::Rank::Seven, Card::Suit::Spades),
        Card(Card::Rank::Two, Card::Suit::Clubs)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::King, Card::Suit::Clubs),
        Card(Card::Rank::Four, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 7);
    assert(value.kickers.size() == 2);
    assert(value.kickers[0] == Card::Rank::King);  // Trips rank
    assert(value.kickers[1] == Card::Rank::Seven); // Pair rank
    std::cout << "Full House test passed." << std::endl;
}

void testFlush() {
    // Flush in spades: A-J-9-6-4
    std::vector<Card> board = {
        Card(Card::Rank::Ace, Card::Suit::Spades),
        Card(Card::Rank::Jack, Card::Suit::Spades),
        Card(Card::Rank::Nine, Card::Suit::Spades),
        Card(Card::Rank::Six, Card::Suit::Spades),
        Card(Card::Rank::King, Card::Suit::Hearts)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Four, Card::Suit::Spades),
        Card(Card::Rank::Two, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 6);
    assert(value.kickers.size() == 5);
    assert(value.kickers[0] == Card::Rank::Ace);  // Highest flush card
    assert(value.kickers[1] == Card::Rank::Jack); // Second highest
    assert(value.kickers[2] == Card::Rank::Nine); // Third highest
    assert(value.kickers[3] == Card::Rank::Six);  // Fourth highest
    assert(value.kickers[4] == Card::Rank::Four); // Fifth highest
    std::cout << "Flush test passed." << std::endl;
}

void testStraight() {
    // Straight: 10-J-Q-K-A
    std::vector<Card> board = {
        Card(Card::Rank::Ten, Card::Suit::Hearts),
        Card(Card::Rank::Jack, Card::Suit::Diamonds),
        Card(Card::Rank::Queen, Card::Suit::Clubs),
        Card(Card::Rank::King, Card::Suit::Spades),
        Card(Card::Rank::Two, Card::Suit::Hearts)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Ace, Card::Suit::Diamonds),
        Card(Card::Rank::Three, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 5);
    assert(value.kickers.size() == 1);
    assert(value.kickers[0] == Card::Rank::Ace); // Top card of straight
    std::cout << "Straight test passed." << std::endl;
}

void testWheelStraight() {
    // Wheel straight: A-2-3-4-5 (Ace low)
    std::vector<Card> board = {
        Card(Card::Rank::Ace, Card::Suit::Hearts),
        Card(Card::Rank::Two, Card::Suit::Diamonds),
        Card(Card::Rank::Three, Card::Suit::Clubs),
        Card(Card::Rank::Four, Card::Suit::Spades),
        Card(Card::Rank::King, Card::Suit::Hearts)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Five, Card::Suit::Diamonds),
        Card(Card::Rank::Seven, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 5);
    assert(value.kickers.size() == 1);
    assert(value.kickers[0] == Card::Rank::Five); // Top card of wheel (5 high)
    std::cout << "Wheel Straight test passed." << std::endl;
}

void testThreeOfAKind() {
    // Three 8s with A and K kickers
    std::vector<Card> board = {
        Card(Card::Rank::Eight, Card::Suit::Hearts),
        Card(Card::Rank::Eight, Card::Suit::Diamonds),
        Card(Card::Rank::Ace, Card::Suit::Clubs),
        Card(Card::Rank::King, Card::Suit::Spades),
        Card(Card::Rank::Four, Card::Suit::Hearts)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Eight, Card::Suit::Clubs),
        Card(Card::Rank::Two, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 4);
    assert(value.kickers.size() == 3);
    assert(value.kickers[0] == Card::Rank::Eight); // Trips rank
    assert(value.kickers[1] == Card::Rank::Ace);   // Highest remaining
    assert(value.kickers[2] == Card::Rank::King);  // Second highest remaining
    std::cout << "Three of a Kind test passed." << std::endl;
}

void testTwoPair() {
    // Aces and 7s with Queen kicker
    std::vector<Card> board = {
        Card(Card::Rank::Ace, Card::Suit::Hearts),
        Card(Card::Rank::Ace, Card::Suit::Diamonds),
        Card(Card::Rank::Seven, Card::Suit::Clubs),
        Card(Card::Rank::Queen, Card::Suit::Spades),
        Card(Card::Rank::Three, Card::Suit::Hearts)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Seven, Card::Suit::Hearts),
        Card(Card::Rank::Two, Card::Suit::Diamonds)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 3);
    assert(value.kickers.size() == 3);
    assert(value.kickers[0] == Card::Rank::Ace);   // Higher pair
    assert(value.kickers[1] == Card::Rank::Seven); // Lower pair
    assert(value.kickers[2] == Card::Rank::Queen); // Highest remaining card
    std::cout << "Two Pair test passed." << std::endl;
}

void testOnePair() {
    // Pair of 4s with A, 9, 7 kickers
    std::vector<Card> board = {
        Card(Card::Rank::Four, Card::Suit::Diamonds),
        Card(Card::Rank::Ace, Card::Suit::Hearts),
        Card(Card::Rank::Nine, Card::Suit::Clubs),
        Card(Card::Rank::Seven, Card::Suit::Spades),
        Card(Card::Rank::Two, Card::Suit::Hearts)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Four, Card::Suit::Spades),
        Card(Card::Rank::Three, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 2);
    assert(value.kickers.size() == 4);
    assert(value.kickers[0] == Card::Rank::Four);  // Pair rank
    assert(value.kickers[1] == Card::Rank::Ace);   // Highest remaining
    assert(value.kickers[2] == Card::Rank::Nine);  // Second highest remaining
    assert(value.kickers[3] == Card::Rank::Seven); // Third highest remaining
    std::cout << "One Pair test passed." << std::endl;
}

void testHighCard() {
    // High card: A-K-J-8-6
    std::vector<Card> board = {
        Card(Card::Rank::Ace, Card::Suit::Hearts),
        Card(Card::Rank::King, Card::Suit::Diamonds),
        Card(Card::Rank::Jack, Card::Suit::Clubs),
        Card(Card::Rank::Eight, Card::Suit::Spades),
        Card(Card::Rank::Four, Card::Suit::Hearts)
    };

    std::vector<Card> hand = {
        Card(Card::Rank::Six, Card::Suit::Diamonds),
        Card(Card::Rank::Two, Card::Suit::Hearts)
    };

    Board pokerBoard(board);
    HandValue value = HandEvaluator::evaluate(pokerBoard, hand);

    assert(value.rankCategory == 1);
    assert(value.kickers.size() == 5);
    assert(value.kickers[0] == Card::Rank::Ace);   // Highest card
    assert(value.kickers[1] == Card::Rank::King);  // Second highest
    assert(value.kickers[2] == Card::Rank::Jack);  // Third highest
    assert(value.kickers[3] == Card::Rank::Eight); // Fourth highest
    assert(value.kickers[4] == Card::Rank::Six);   // Fifth highest
    std::cout << "High Card test passed." << std::endl;
}

int main() {
    std::cout << "Running comprehensive hand evaluation tests..." << std::endl;
    std::cout << "=============================================" << std::endl;
    
    testStraightFlush();
    testFourOfAKind();
    testFullHouse();
    testFlush();
    testStraight();
    testWheelStraight();
    testThreeOfAKind();
    testTwoPair();
    testOnePair();
    testHighCard();
    
    std::cout << "=============================================" << std::endl;
    std::cout << "All tests passed successfully!" << std::endl;
    
    return 0;
}
