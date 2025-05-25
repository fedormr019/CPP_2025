#include "Paddle.h"
#include <SDL.h>

Paddle::Paddle(SDL_Renderer* rend, int x, int y) : renderer(rend), speed(10) {
    rect = { x - 50, y, 100, 20 };
}

void Paddle::update() {
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_LEFT]) rect.x -= speed;
    if (keystates[SDL_SCANCODE_RIGHT]) rect.x += speed;

    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.w > 800) rect.x = 800 - rect.w;
}

void Paddle::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Paddle::setWidth(int newWidth) {
    rect.w = newWidth;
}

void Paddle::increaseWidth(int delta) {
    rect.w += delta;
    if (rect.w > 300) rect.w = 300;
}

int Paddle::getX() const { return rect.x + rect.w / 2; }
int Paddle::getY() const { return rect.y; }
SDL_Rect Paddle::getRect() const { return rect; }