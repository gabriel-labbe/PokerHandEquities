#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

class Card {
public:
    enum class Rank { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
    enum class Suit { Hearts, Diamonds, Clubs, Spades };

    Card(Rank rank, Suit suit);

    Rank getRank() const;
    void setRank(Rank rank);

    Suit getSuit() const;
    void setSuit(Suit suit);

    std::string getString() const;

    int getId() const;

    friend std::ostream& operator<<(std::ostream& os, const Card& card);

private:
    Rank rank;
    Suit suit;

    char rankToChar() const;
    char suitToChar() const;
};

#endif
