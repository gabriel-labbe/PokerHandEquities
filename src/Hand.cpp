#include "../include/Hand.h"
#include <algorithm>

Hand::Hand(Card c1, Card c2) : card1(c1), card2(c2), mask(0) {
    if (static_cast<int>(card1.getRank()) < static_cast<int>(card2.getRank())) {
        std::swap(card1, card2);
    }
    mask = (1ULL << card1.getId()) | (1ULL << card2.getId());
}

Card Hand::getCard1() const {
    return card1;
}

Card Hand::getCard2() const {
    return card2;
}

uint64_t Hand::getMask() const {
    return mask;
}

std::vector<Card> Hand::getCards() const {
    return {card1, card2};
}
