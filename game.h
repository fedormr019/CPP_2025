#pragma once
#include <SDL.h>
#include <vector>
#include "Ball.h"
#include "Paddle.h"
#include "Block.h"
#include "Bonus.h"
#include <SDL_ttf.h>

extern TTF_Font* font;
extern SDL_Texture* scoreTexture;
extern SDL_Rect scoreRect;

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void cleanup();
    bool hasSafetyBottom = false;
    SDL_Rect safetyBottomRect = { 0, 590, 800, 10 }; 

private:
    void handleEvents();
    void update();
    void render();
    int lives = 3;
    int score = 0;
    void updateScoreTexture();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    Paddle* paddle;
    std::vector<Ball*> balls;
    std::vector<Block*> blocks;
    std::vector<Bonus*> bonuses;

    void spawnSecondBall(); 
};