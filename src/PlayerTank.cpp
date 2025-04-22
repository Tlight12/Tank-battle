#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Bullet.h"
#include <cmath>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;
PlayerTank::PlayerTank(){
    x = 0;
    y = 0;
    dirX = 0;
    dirY = -1;
    isAlive = true;
    texture = nullptr;
    exploding = false;
    explodeTimer = 0;
    explosionTexture = nullptr;
    rect = { x, y, TILE_SIZE, TILE_SIZE };
}

PlayerTank::PlayerTank(int startX, int startY){
    x = startX;
    y = startY;
    dirX = 0;
    dirY = -1;
    isAlive = true;
    texture = nullptr;
    exploding = false;
    explodeTimer = 0;
    explosionTexture = nullptr;
    rect = { x, y, TILE_SIZE, TILE_SIZE };
}

void PlayerTank::move(int dx, int dy, const std::vector<Wall>& walls,
                      const std::vector<EnemyTank>& enemies, const PlayerTank& otherPlayer)
{
    int newX = x + dx;
    int newY = y + dy;
    dirX = dx;
    dirY = dy;
    if (!isAlive)
        return;
    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };

    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect))
            return;
    }
    for (const auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&newRect, &enemy.rect))
            return;
    }
    if (otherPlayer.isAlive && SDL_HasIntersection(&newRect, &otherPlayer.rect))
        return;

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}

void PlayerTank::shoot() {
    if (!isAlive) return;
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5, dirX, dirY));
}

void PlayerTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [] (Bullet &b) { return !b.active; }), bullets.end());
}

void PlayerTank::loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

double PlayerTank::getAngle() const {
    if (dirX == 0 && dirY == 0) return 0.0;
    double angle = atan2(dirY, dirX) * 180.0 / M_PI;
    return angle + 90.0;
}

void PlayerTank::render(SDL_Renderer* renderer) {
    if (exploding) {
        if (explosionTexture) {
            SDL_RenderCopy(renderer, explosionTexture, nullptr, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        return;
    }
    if (!isAlive) return;
    if (texture) {
        SDL_RenderCopyEx(renderer, texture, nullptr, &rect, getAngle(), nullptr, SDL_FLIP_NONE);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}
