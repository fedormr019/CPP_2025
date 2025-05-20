#ifndef BONUS_H
#define BONUS_H

#include "grid.h"

void applyBonus(Field& field, int x, int y, int originColor);
std::vector<std::pair<int, int>> getRadiusNeighbors(int x, int y, int radius);

#endif