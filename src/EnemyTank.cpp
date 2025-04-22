#include "EnemyTank.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;
EnemyTank::EnemyTank(int startX, int startY) {
    moveDelay = 10;
    shootDelay = 5;
    x = startX;
    y = startY;
    texture = nullptr;
    exploding = false;
    explodeTimer = 0;
    explosionTexture = nullptr;
    rect = { x, y, TILE_SIZE, TILE_SIZE };
    dirX = 0;
    dirY = 1;
    active = true;
}

void EnemyTank::move(const vector<Wall>& walls, const vector<EnemyTank>& enemies, const PlayerTank& player, const PlayerTank& player2) {
    if (--moveDelay > 0) return;
    moveDelay = 10;
    int r = rand() % 4;
    if (r == 0) { // Up
        this->dirX = 0;
        this->dirY = -10;
    }
    else if (r == 1) { // Down
        this->dirX = 0;
        this->dirY = 10;
    }
    else if (r == 2) { // Left
        this->dirY = 0;
        this->dirX = -10;
    }
    else if (r == 3) { // Right
        this->dirY = 0;
        this->dirX = 10;
    }
    int newX = x + this->dirX;
    int newY = y + this->dirY;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
            return;
        }
    }
    for (const auto& enemy : enemies) {
        if (&enemy != this && enemy.active && SDL_HasIntersection(&newRect, &enemy.rect)) {
            return;
        }
    }
    if (player.isAlive && SDL_HasIntersection(&newRect, &player.rect)) return;
    if (player2.isAlive && SDL_HasIntersection(&newRect, &player2.rect)) return;

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}

void EnemyTank::shoot() {
    if (--shootDelay > 0) return;
    shootDelay = 5;
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5, dirX, dirY));
}

void EnemyTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet &b) { return !b.active; }), bullets.end());
}
void EnemyTank::loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

double EnemyTank::getAngle() const {
    if (dirX == 0 && dirY == 0) return 0.0;
    double angle = atan2(dirY, dirX) * 180.0 / M_PI;
    return angle + 90.0;
}

void EnemyTank::render(SDL_Renderer* renderer) {
    if (exploding) {
        if (explosionTexture) {
            SDL_RenderCopy(renderer, explosionTexture, nullptr, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        return;
    }
    if (!active) return;
    if (texture) {
        SDL_RenderCopyEx(renderer, texture, nullptr, &rect, getAngle(), nullptr, SDL_FLIP_NONE);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}
