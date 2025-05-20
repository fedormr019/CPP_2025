#include <SDL.h>
#include <iostream>
#include "grid.h"
#include "bonus.h"
#include "animation.h"
#include <ctime>
#include <vector>
#include <utility>

const int SCREEN_WIDTH = COLS * CELL_SIZE;
const int SCREEN_HEIGHT = ROWS * CELL_SIZE;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "initialize error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("GEMS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "creating window error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "creating renderer error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    srand(static_cast<unsigned>(time(0)));
    Field field = createField();

    bool quit = false;
    SDL_Event e;
    int clickX = -1, clickY = -1;

    while (!quit) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        drawGrid(renderer, field);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.y / CELL_SIZE;
                int y = e.button.x / CELL_SIZE;

                if (clickX == -1) {
                    clickX = x;
                    clickY = y;
                }
                else {
                    if (swapCells(field, clickX, clickY, x, y)) {
                        animateSwap(renderer, field, clickX, clickY, x, y);

                        auto matches = findMatches(field);
                        if (matches.empty()) {
                            swapCells(field, clickX, clickY, x, y);
                        }
                        else {
                            while (!matches.empty()) {
                                for (auto [mx, my] : matches) {
                                    applyBonus(field, mx, my, field[mx][my].color);
                                    field[mx][my].color = -1;
                                }

                                auto movedCells = applyGravity(field);
                                if (!movedCells.empty()) {
                                    animateFalling(renderer, field, movedCells);
                                }

                                matches = findMatches(field);
                            }
                        }
                    }
                    clickX = clickY = -1;
                }
            }
        }

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
