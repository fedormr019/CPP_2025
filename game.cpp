#include "Game.h"
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <sstream>

TTF_Font* font = nullptr;
SDL_Texture* scoreTexture = nullptr;
SDL_Rect scoreRect;

Game::Game() {}

Game::~Game() {
    for (auto ball : balls) delete ball;
    delete paddle;
    for (auto block : blocks) delete block;
    for (auto bonus : bonuses) delete bonus;
}

bool Game::init(const char* title, int width, int height) {
    if (TTF_Init() == -1) {
        std::cout << "TTF_Init error: " << TTF_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24); 

    if (!font) {
        std::cout << "Font load error: " << TTF_GetError() << std::endl;
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);
    if (!window) return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    paddle = new Paddle(renderer, width / 2, height - 50);
    balls.push_back(new Ball(renderer, width / 2, height / 2));

    srand(static_cast<unsigned int>(time(nullptr)));

    int rows = 6;
    int cols = 12;
    int blockWidth = 60;
    int blockHeight = 20;
    int spacingX = 5;
    int spacingY = 4;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = col * (blockWidth + spacingX) + 30;
            int y = row * (blockHeight + spacingY) + 50;

            int type = rand() % 8;

            bool indestructible = false;
            bool speedBlock = false;
            BonusType bonus = BonusType::None;

            switch (type) {
            case 0: bonus = BonusType::SecondBall; break;
            case 1: bonus = BonusType::BallSpeedUp; break;
            case 2: bonus = BonusType::PaddleSizeUp; break;
            case 3: bonus = BonusType::SafetyBottom; break;
            case 4: bonus = BonusType::StickyBall; break; 
            case 5: speedBlock = true; break;
            case 6: indestructible = true; break;
            case 7: break; 
            }

            int health = indestructible ? 999 : ([]() {
                int r = rand() % 10;
                if (r < 4) return 1;  
                if (r < 7) return 2; 
                return 3;              
                })();

                blocks.push_back(new Block(renderer, x, y, health, indestructible, bonus, speedBlock));
        }
    }
    safetyBottomRect = { 0, 590, 800, 10 };

    running = true;
    return true;
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
    }
}

void Game::update() {
    paddle->update();
    bool usedSafety = false;
    float factor = (rand() % 2 == 0) ? 1.3f : 0.7f;
    for (auto& ball : balls) {
        ball->update(paddle, blocks, bonuses, hasSafetyBottom, usedSafety, score);
    }
    if (usedSafety) {
        hasSafetyBottom = false; 
    }

    for (auto& bonus : bonuses) {
        bonus->update();
    }

    for (auto& bonus : bonuses) {
        bonus->update();

        if (bonus->isCollected(paddle->getRect())) {
            switch (bonus->getType()) {
            case BonusType::SecondBall:
                spawnSecondBall();
                break;
            case BonusType::PaddleSizeUp: {
                static int sizeBonusEffect = 50;
                paddle->increaseWidth(sizeBonusEffect);
                if (sizeBonusEffect > 10) sizeBonusEffect -= 10; 
                break;
            }
            case BonusType::BallSpeedUp: {
                for (auto& ball : balls) {
                    ball->multiplySpeed(factor);
                }
                break;
            }
            case BonusType::StickyBall:
                for (auto& ball : balls) {
                    ball->enableSticky();
                }
                break;
            case BonusType::SafetyBottom:
                hasSafetyBottom = true;
                break;
            default:
                break;
            }

            bonus->setActive(false);
        }
    }

    static int lastShownScore = -1;
    if (score != lastShownScore) {
        updateScoreTexture();
        lastShownScore = score;
    }

    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            SDL_Rect a = balls[i]->getRect();
            SDL_Rect b = balls[j]->getRect();

            if (SDL_HasIntersection(&a, &b)) {
                balls[i]->invertDirection();
                balls[j]->invertDirection();
            }
        }
    }

    bonuses.erase(std::remove_if(bonuses.begin(), bonuses.end(),
        [](Bonus* b) { return !b->isActive(); }), bonuses.end());

    balls.erase(
        std::remove_if(balls.begin(), balls.end(),
            [](Ball* b) { return b->isDead(); }),
        balls.end());

    for (auto it = balls.begin(); it != balls.end();) {
        if ((*it)->isDead()) {
            delete* it;
            it = balls.erase(it);
        }
        else {
            ++it;
        }
    }

    if (balls.empty()) {
        lives--;
        score -= 10;
        if (lives == 0) {
            running = false;  
        }
        else {
            balls.push_back(new Ball(renderer, paddle->getX(), paddle->getY() - 20));
        }
    }

}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    paddle->render();
    for (auto ball : balls) ball->render();
    for (auto block : blocks) block->render();
    for (auto bonus : bonuses) bonus->render();

    if (hasSafetyBottom) {
        SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); 
        SDL_RenderFillRect(renderer, &safetyBottomRect);
    }

    if (scoreTexture) {
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    }

    SDL_RenderPresent(renderer);
}

void Game::spawnSecondBall() {
    Ball* newBall = new Ball(renderer, paddle->getX(), paddle->getY() - 20);
    newBall->setVelocity(3, -3);
    balls.push_back(newBall);
}

void Game::updateScoreTexture() {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);

    SDL_Color color = { 255, 255, 255, 255 };
    std::ostringstream ss;
    ss << "Score: " << score << "   Lives: " << lives;
    std::string scoreText = ss.str();
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), color);
    scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    scoreRect = { 10, 10, surface->w, surface->h };
    SDL_FreeSurface(surface);
}

void Game::cleanup() {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}