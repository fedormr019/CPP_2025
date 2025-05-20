#ifndef GRID_H
#define GRID_H

#include <vector>
#include <SDL.h>

const int ROWS = 8;
const int COLS = 8;
const int CELL_SIZE = 64;
const int COLOR_COUNT = 5;

enum BonusType { NONE, PAINT, BOMB };

struct Cell {
    int color;
    BonusType bonus = NONE;
};

using Field = std::vector<std::vector<Cell>>;

Field createField();
void drawGrid(SDL_Renderer* renderer, const Field& field);
bool swapCells(Field& field, int x1, int y1, int x2, int y2);
std::vector<std::pair<int, int>> findMatches(const Field& field);
std::vector<std::pair<int, int>> applyGravity(Field& field);
SDL_Color getColor(int colorIndex);

#endif