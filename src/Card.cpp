#include "../include/Card.h"
#include <iostream>

Card::Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}

Card::Rank Card::getRank() const {
    return rank;
}

void Card::setRank(Rank rank) {
    this->rank = rank;
}

Card::Suit Card::getSuit() const {
    return suit;
}

void Card::setSuit(Suit suit) {
    this->suit = suit;
}

char Card::rankToChar() const {
    switch (rank) {
        case Rank::Two:   return '2';
        case Rank::Three: return '3';
        case Rank::Four:  return '4';
        case Rank::Five:  return '5';
        case Rank::Six:   return '6';
        case Rank::Seven: return '7';
        case Rank::Eight: return '8';
        case Rank::Nine:  return '9';
        case Rank::Ten:   return 'T';
        case Rank::Jack:  return 'J';
        case Rank::Queen: return 'Q';
        case Rank::King:  return 'K';
        case Rank::Ace:   return 'A';
        default:          return '?';
    }
}

char Card::suitToChar() const {
    switch (suit) {
        case Suit::Hearts:   return 'h';
        case Suit::Diamonds: return 'd';
        case Suit::Clubs:    return 'c';
        case Suit::Spades:   return 's';
        default:             return '?';
    }
}

std::string Card::getString() const {
    return std::string(1, rankToChar()) + suitToChar();
}

int Card::getId() const {
    int rankVal = static_cast<int>(rank) - 2; // 0-12 for Two-Ace
    int suitVal = static_cast<int>(suit); // 0-3
    return suitVal * 13 + rankVal;
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    os << card.getString();
    return os;
}
