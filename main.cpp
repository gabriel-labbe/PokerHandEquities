#include <iostream>
#include "Card.h"
#include "Board.h"
#include "HandEvaluator.h"

int main() {
    // Create a board with 5 cards: Ah, Kd, Qs, Jh, Tc (Royal Straight possible!)
    std::vector<Card> boardCards = {
        Card(Card::Rank::Ace, Card::Suit::Hearts),
        Card(Card::Rank::Ace, Card::Suit::Diamonds),
        Card(Card::Rank::Queen, Card::Suit::Spades),
        Card(Card::Rank::Jack, Card::Suit::Hearts),
        Card(Card::Rank::Ten, Card::Suit::Clubs)
    };

    Board board(boardCards);

    // Use the new Board methods
    std::cout << "Is flush possible: " << (board.isFlushPossible() ? "Yes" : "No") << std::endl;
    std::cout << "Is straight possible: " << (board.isStraightPossible() ? "Yes" : "No") << std::endl;
    std::cout << "Is board paired: " << (board.isPaired() ? "Yes" : "No") << std::endl;

    // Test another board
    std::vector<Card> pairedBoard = {
        Card(Card::Rank::Two, Card::Suit::Hearts),
        Card(Card::Rank::Ten, Card::Suit::Diamonds),
        Card(Card::Rank::Four, Card::Suit::Spades),
        Card(Card::Rank::Eight, Card::Suit::Clubs),
        Card(Card::Rank::Two, Card::Suit::Hearts)
    };

    Board board2(pairedBoard);
    std::cout << "\nSecond board:" << std::endl;
    std::cout << "Is flush possible: " << (board2.isFlushPossible() ? "Yes" : "No") << std::endl;
    std::cout << "Is straight possible: " << (board2.isStraightPossible() ? "Yes" : "No") << std::endl;
    std::cout << "Is board paired: " << (board2.isPaired() ? "Yes" : "No") << std::endl;

    return 0;
}
