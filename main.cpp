#include "Game.h"

int main(int argc, char* argv[]) {
    Game game;
    if (game.init("Arkanoid", 800, 600)) {
        game.run();
    }
    game.cleanup();
    return 0;
}