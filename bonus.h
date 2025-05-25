#pragma once
#include <SDL.h>

enum class BonusType {
    None,
    SecondBall,     
    PaddleSizeUp,  
    BallSpeedUp,     
    StickyBall,
    SafetyBottom    
};

class Bonus {
public:
    Bonus(SDL_Renderer* renderer, int x, int y, BonusType type);
    void update();
    void render();
    bool isCollected(SDL_Rect paddleRect);
    bool isActive() const;
    void setActive(bool val);
    BonusType getType() const;

private:
    SDL_Renderer* renderer;
    SDL_Rect rect;
    int speed;
    bool active;
    BonusType type;
};