#pragma once
#include <SDL.h>

class Paddle {
public:
    Paddle(SDL_Renderer* renderer, int x, int y);
    void update();
    void render();
    int getX() const;
    int getY() const;
    SDL_Rect getRect() const;
    void setWidth(int newWidth);
    void increaseWidth(int delta);

private:
    SDL_Renderer* renderer;
    SDL_Rect rect;
    int speed;
};