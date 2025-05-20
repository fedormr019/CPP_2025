#include "bonus.h"
#include <cstdlib>

std::vector<std::pair<int, int>> getRadiusNeighbors(int x, int y, int radius) {
    std::vector<std::pair<int, int>> result;
    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS && (dx || dy))
                result.push_back({ nx, ny });
        }
    }
    return result;
}

void applyBonus(Field& field, int x, int y, int originColor) {
    if (rand() % 4 == 0) {
        field[x][y].bonus = (rand() % 2) ? PAINT : BOMB;
        if (field[x][y].bonus == PAINT) {
            auto nearby = getRadiusNeighbors(x, y, 3);
            field[x][y].color = originColor;
            for (int i = 0; i < 2 && !nearby.empty(); ++i) {
                int idx = rand() % nearby.size();
                auto [nx, ny] = nearby[idx];
                field[nx][ny].color = originColor;
                nearby.erase(nearby.begin() + idx);
            }
        }
        else {
            auto nearby = getRadiusNeighbors(x, y, 3);
            nearby.push_back({ x, y });
            for (int i = 0; i < 5 && !nearby.empty(); ++i) {
                int idx = rand() % nearby.size();
                auto [bx, by] = nearby[idx];
                field[bx][by].color = -1;
                nearby.erase(nearby.begin() + idx);
            }
        }
    }
}