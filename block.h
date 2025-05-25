#pragma once
#include <SDL.h>
#include "Bonus.h"

class Block {
public:
    Block(SDL_Renderer* renderer, int x, int y, int health, bool indestructible = false,
        BonusType bonus = BonusType::None, bool speedBlock = false);
    void render();
    void hit();
    bool isAlive() const;
    bool hasBonus() const;
    bool wasJustDestroyed();
    bool isIndestructible() const;
    bool isSpeedBlock() const;
    SDL_Rect getRect() const;
    BonusType getBonusType() const;

private:
    SDL_Renderer* renderer;
    SDL_Rect rect;
    int health;
    bool indestructible;
    bool justDestroyed = false;
    bool speedBlock = false;
    BonusType bonusType;
};