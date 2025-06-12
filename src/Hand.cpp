#include "../include/Hand.h"

Hand::Hand(Card c1, Card c2) : card1(c1), card2(c2) {
    mask = (1ULL << c1.getId()) | (1ULL << c2.getId());
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
