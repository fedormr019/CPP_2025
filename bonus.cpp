#include "Bonus.h"

Bonus::Bonus(SDL_Renderer* rend, int x, int y, BonusType t) : renderer(rend), speed(3), active(true), type(t) {
    rect = { x + 20, y, 20, 20 };
}

void Bonus::update() {
    rect.y += speed;
    if (rect.y > 600) active = false;
}

void Bonus::render() {
    if (!active) return;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool Bonus::isCollected(SDL_Rect paddleRect) {
    return active && SDL_HasIntersection(&rect, &paddleRect);
}

bool Bonus::isActive() const { return active; }
void Bonus::setActive(bool val) { active = val; }
BonusType Bonus::getType() const { return type; }