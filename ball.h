#pragma once
#include <SDL.h>
#include <vector>
#include "Paddle.h"
#include "Block.h"
#include "Bonus.h"

class Ball {
public:
    Ball(SDL_Renderer* renderer, int x, int y);
    void update(Paddle* paddle, std::vector<Block*>& blocks, std::vector<Bonus*>& bonuses, bool hasSafetyBottom, bool& safetyUsed, int& score);
    void render();
    void setVelocity(int vx, int vy);
    bool isDead() const;
    void multiplySpeed(float factor);
    SDL_Rect getRect() const;
    void invertDirection();
    bool stickyEnabled = false;
    bool stuckToPaddle = false;
    int offsetFromPaddle = 0;
    Paddle* paddleRef = nullptr;

    void enableSticky();
    void stickToPaddle(Paddle* paddle);
    void releaseFromPaddle();

private:
    SDL_Renderer* renderer;
    SDL_Rect rect;
    int velX, velY;
};