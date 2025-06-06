#ifndef EQUITYCALCULATOR_H
#define EQUITYCALCULATOR_H

#include "Card.h"
#include "HandEvaluator.h"
#include <vector>

class EquityCalculator {
public:
static double calculateEquity(const std::vector<std::vector<Card>>& group1, const std::vector<std::vector<Card>>& group2, int iterations);
};

#endif
