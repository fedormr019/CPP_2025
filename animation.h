#ifndef ANIMATION_H
#define ANIMATION_H

#include "grid.h"
#include <SDL.h>

void animateSwap(SDL_Renderer* renderer, Field& field, int x1, int y1, int x2, int y2);
void animateFalling(SDL_Renderer* renderer, Field& field, const std::vector<std::pair<int, int>>& movedCells);

#endif