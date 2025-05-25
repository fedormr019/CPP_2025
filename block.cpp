#include "Block.h"

Block::Block(SDL_Renderer* rend, int x, int y, int hp, bool ind, BonusType bonus, bool spd)
    : renderer(rend), health(hp), indestructible(ind), bonusType(bonus), speedBlock(spd) {
    rect = { x, y, 60, 20 };
}

void Block::render() {
    if (!isAlive()) return;

    if (speedBlock) {
        SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
    }
    else if (indestructible) {
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    }
    else {
        switch (health) {
        case 3:
            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
            break;
        case 2:
            SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
            break;
        case 1:
            SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            break;
        }
    }

    SDL_RenderFillRect(renderer, &rect);
}

void Block::hit() {
    if (!indestructible && health > 0) {
        health--;
        if (health == 0) {
            justDestroyed = true;
        }
    }
}

bool Block::isSpeedBlock() const {
    return speedBlock;
}

bool Block::isIndestructible() const {
    return indestructible;
}

bool Block::wasJustDestroyed() {
    if (justDestroyed) {
        justDestroyed = false;
        return true;
    }
    return false;
}

bool Block::isAlive() const { return indestructible || health > 0; }
bool Block::hasBonus() const { return bonusType != BonusType::None; }
SDL_Rect Block::getRect() const { return rect; }
BonusType Block::getBonusType() const { return bonusType; }