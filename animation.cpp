#include "animation.h"
#include <map>
#include <SDL.h>

void animateSwap(SDL_Renderer* renderer, Field& field, int x1, int y1, int x2, int y2) {
    const int duration = 200;
    Uint32 start = SDL_GetTicks();
    SDL_Rect rect{ 0, 0, CELL_SIZE, CELL_SIZE };

    while (SDL_GetTicks() - start < duration) {
        float t = (SDL_GetTicks() - start) / (float)duration;
        SDL_RenderClear(renderer);

        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                rect.x = j * CELL_SIZE;
                rect.y = i * CELL_SIZE;

                if ((i == x1 && j == y1) || (i == x2 && j == y2)) {
                    float offset = t * CELL_SIZE;
                    rect.x += (j == y1 ? (y2 - y1) : (y1 - y2)) * offset;
                    rect.y += (i == x1 ? (x2 - x1) : (x1 - x2)) * offset;
                }

                if (field[i][j].color != -1) {
                    SDL_Color c = getColor(field[i][j].color);
                    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void animateFalling(SDL_Renderer* renderer, Field& field, const std::vector<std::pair<int, int>>& movedCells) {
    const int duration = 300;
    Uint32 start = SDL_GetTicks();
    std::map<std::pair<int, int>, std::pair<int, int>> movementMap;

    for (const auto& [oldRow, oldCol] : movedCells) {
        if (oldRow == -1) {
            for (int i = 0; i < ROWS; ++i) {
                if (field[i][oldCol].color != -1) {
                    movementMap[{i, oldCol}] = { -1, oldCol };
                    break;
                }
            }
        }
        else {
            int newRow = oldRow;
            while (newRow + 1 < ROWS && field[newRow + 1][oldCol].color == -1) {
                newRow++;
            }
            movementMap[{newRow, oldCol}] = { oldRow, oldCol };
        }
    }

    while (SDL_GetTicks() - start < duration) {
        float progress = (SDL_GetTicks() - start) / (float)duration;
        SDL_RenderClear(renderer);

        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (!movementMap.count({ i, j }) && field[i][j].color != -1) {
                    SDL_Rect rect{ j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                    SDL_Color c = getColor(field[i][j].color);
                    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }
        }

        for (const auto& [newPos, oldPos] : movementMap) {
            auto [i, j] = newPos;
            SDL_Rect rect{ j * CELL_SIZE, 0, CELL_SIZE, CELL_SIZE };

            if (oldPos.first == -1) {
                rect.y = -CELL_SIZE + progress * (i + 1) * CELL_SIZE;
            }
            else {
                rect.y = oldPos.first * CELL_SIZE + progress * (i - oldPos.first) * CELL_SIZE;
            }

            SDL_Color c = getColor(field[i][j].color);
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}