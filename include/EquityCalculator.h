#ifndef EQUITYCALCULATOR_H
#define EQUITYCALCULATOR_H

#include "Card.h"
#include "HandEvaluator.h"
#include <vector>

class EquityCalculator {
public:
    static double calculateEquityMonteCarlo(const std::vector<Card>& hand1, const std::vector<Card>& hand2, int iterations);
    static double calculateExactEquity(const std::vector<Card>& hand1, const std::vector<Card>& hand2);
    static double calculateExactEquityRecursive(const std::vector<Card>& hand1, const std::vector<Card>& hand2);
    static double calculateExactEquityPermutation(const std::vector<Card>& hand1, const std::vector<Card>& hand2);
};

#endif
