#ifndef EQUITYCALCULATOR_H
#define EQUITYCALCULATOR_H

#include "Card.h"
#include "HandEvaluator.h"
#include <vector>

class EquityCalculator {
public:
    static double calculateEquity(const std::vector<Card>& hand1, const std::vector<Card>& hand2, int iterations);
    static double calculateExactEquity(const std::vector<Card>& hand1, const std::vector<Card>& hand2);
};

#endif
