#include "PlayerTank.h"
#include <algorithm>
using namespace std;
PlayerTank::PlayerTank() {
    x = 0;
    y = 0;
    dirX = 0;
    dirY = -1;
    isAlive = true;
    rect = { x, y, TILE_SIZE, TILE_SIZE };
}

PlayerTank::PlayerTank(int startX, int startY) {
    x = startX;
    y = startY;
    isAlive = true;
    rect = { x, y, TILE_SIZE, TILE_SIZE };
    dirX = 0;
    dirY = -1; // hướng mặc định là lên trên
}

void PlayerTank::move(int dx, int dy, const vector<Wall>& walls) {
    int newX = x + dx;
    int newY = y + dy;
    dirX = dx;
    dirY = dy;
    if (!isAlive) return;
    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect))
            return;
    }
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
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet &b) { return !b.active; }), bullets.end());
}

void PlayerTank::render(SDL_Renderer* renderer) {
    if (!isAlive) return;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}
