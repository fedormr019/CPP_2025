#include "Ball.h"
#include <SDL.h>
#include <algorithm>

Ball::Ball(SDL_Renderer* rend, int x, int y) : renderer(rend), velX(4), velY(-4) {
    rect = { x - 8, y - 8, 16, 16 };
}

void Ball::setVelocity(int vx, int vy) {
    velX = vx;
    velY = vy;
}

void Ball::update(Paddle* paddle, std::vector<Block*>& blocks, std::vector<Bonus*>& bonuses, bool hasSafetyBottom, bool& safetyUsed, int& score) {
    rect.x += velX;
    rect.y += velY;
    paddleRef = paddle;

    if (stuckToPaddle) {
        rect.x = paddle->getX() + offsetFromPaddle;

        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_SPACE]) {
            releaseFromPaddle();
        }

        return; 
    }

    if (rect.y > 600) {
        if (hasSafetyBottom && !safetyUsed) {
            safetyUsed = true;
            rect.y = 580;
            velY = -abs(velY);
            return;
        }
        return;
    }

    if (rect.x <= 0 || rect.x + rect.w >= 800) velX = -velX;
    if (rect.y <= 0) velY = -velY;

    SDL_Rect paddleRect = paddle->getRect();
    if (SDL_HasIntersection(&rect, &paddleRect)) {
        rect.y = paddle->getRect().y - rect.h;
        velY = -abs(velY);

        if (stickyEnabled) {
            stickToPaddle(paddle);
            return;
        }
    }

    for (auto& block : blocks) {
        if (!block->isAlive()) continue;

        SDL_Rect blockRect = block->getRect();

        if (SDL_HasIntersection(&rect, &blockRect)) {
            int left = (rect.x + rect.w) - blockRect.x;
            int right = (blockRect.x + blockRect.w) - rect.x;
            int top = (rect.y + rect.h) - blockRect.y;
            int bottom = (blockRect.y + blockRect.h) - rect.y;

            int minHor = std::min(left, right);
            int minVer = std::min(top, bottom);

            if (minHor < minVer) {
                velX = -velX;
            }
            else {
                velY = -velY;
            }

            block->hit();
            if (!block->isIndestructible()) {
                score++;
            }

            if (block->isSpeedBlock()) {
                multiplySpeed(1.3f);
            }

            if (block->wasJustDestroyed() && block->hasBonus()) {
                bonuses.push_back(new Bonus(renderer, blockRect.x, blockRect.y, block->getBonusType()));
            }

            break;
        }
    }

    blocks.erase(
        std::remove_if(blocks.begin(), blocks.end(),
            [](Block* b) { return !b->isAlive(); }),
        blocks.end());
}

void Ball::multiplySpeed(float factor) {
    velX = static_cast<int>(velX * factor);
    velY = static_cast<int>(velY * factor);

    if (velX == 0) velX = 1;
    if (velY == 0) velY = 1;
    if (velX > 10) velX = 10;
    if (velY > 10) velY = 10;
}

SDL_Rect Ball::getRect() const {
    return rect;
}

void Ball::invertDirection() {
    velX = -velX;
    velY = -velY;
}

void Ball::enableSticky() {
    stickyEnabled = true;
}

void Ball::stickToPaddle(Paddle* paddle) {
    stuckToPaddle = true;
    stickyEnabled = false;
    offsetFromPaddle = rect.x - paddle->getX();
    velX = 0;  
    velY = 0;
}

void Ball::releaseFromPaddle() {
    stuckToPaddle = false;

    int paddleCenter = paddleRef->getRect().x + paddleRef->getRect().w / 2;
    int ballCenter = rect.x + rect.w / 2;

    int dx = ballCenter - paddleCenter;

    velX = dx / 5;  
    if (velX == 0) velX = 1; 
    velY = -5;  
}

void Ball::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool Ball::isDead() const {
    return rect.y > 600;
}
