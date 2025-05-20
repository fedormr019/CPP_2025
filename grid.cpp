#include "grid.h"
#include <cstdlib>

int randomColor() { return rand() % COLOR_COUNT; }

Field createField() {
    Field field(ROWS, std::vector<Cell>(COLS));
    for (auto& row : field) {
        for (auto& cell : row) {
            cell.color = randomColor();
        }
    }
    return field;
}

void drawGrid(SDL_Renderer* renderer, const Field& field) {
    SDL_Rect rect{ 0, 0, CELL_SIZE, CELL_SIZE };

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            rect.x = j * CELL_SIZE;
            rect.y = i * CELL_SIZE;

            if (field[i][j].color != -1) {
                SDL_Color c = getColor(field[i][j].color);
                SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

bool swapCells(Field& field, int x1, int y1, int x2, int y2) {
    if (abs(x1 - x2) + abs(y1 - y2) != 1) return false;
    std::swap(field[x1][y1], field[x2][y2]);
    return true;
}

std::vector<std::pair<int, int>> findMatches(const Field& field) {
    std::vector<std::pair<int, int>> matches;
    std::vector<std::vector<bool>> marked(ROWS, std::vector<bool>(COLS, false));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS - 2; ++j) {
            if (field[i][j].color != -1 &&
                field[i][j].color == field[i][j + 1].color &&
                field[i][j].color == field[i][j + 2].color) {
                for (int k = 0; k < 3; ++k) {
                    if (!marked[i][j + k]) {
                        matches.emplace_back(i, j + k);
                        marked[i][j + k] = true;
                    }
                }
            }
        }
    }

    for (int j = 0; j < COLS; ++j) {
        for (int i = 0; i < ROWS - 2; ++i) {
            if (field[i][j].color != -1 &&
                field[i][j].color == field[i + 1][j].color &&
                field[i][j].color == field[i + 2][j].color) {
                for (int k = 0; k < 3; ++k) {
                    if (!marked[i + k][j]) {
                        matches.emplace_back(i + k, j);
                        marked[i + k][j] = true;
                    }
                }
            }
        }
    }

    return matches;
}

std::vector<std::pair<int, int>> applyGravity(Field& field) {
    std::vector<std::pair<int, int>> movedCells;

    for (int j = 0; j < COLS; ++j) {
        for (int i = ROWS - 1; i >= 0; --i) {
            if (field[i][j].color == -1) continue;

            int newRow = i;
            while (newRow + 1 < ROWS && field[newRow + 1][j].color == -1) {
                newRow++;
            }

            if (newRow != i) {
                movedCells.emplace_back(i, j);
                field[newRow][j] = field[i][j];
                field[i][j].color = -1;
            }
        }

        for (int i = 0; i < ROWS; ++i) {
            if (field[i][j].color == -1) {
                field[i][j].color = randomColor();
                movedCells.emplace_back(-1, j);
            }
        }
    }

    return movedCells;
}

SDL_Color getColor(int colorIndex) {
    static const SDL_Color colors[COLOR_COUNT] = {
        {255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255},
        {255, 255, 0, 255}, {255, 0, 255, 255}
    };
    return colors[colorIndex % COLOR_COUNT];
}