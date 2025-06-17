#ifndef EQUITYCALCULATOR_H
#define EQUITYCALCULATOR_H

#include "Card.h"
#include "Hand.h"
#include "HandEvaluator.h"
#include <vector>

class EquityCalculator {
public:
    static double calculateEquityMonteCarlo(const Hand& hand1, const Hand& hand2, int iterations);
    static double calculateExactEquity(const Hand& hand1, const Hand& hand2);
};

#endif
