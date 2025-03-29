#include "EnemyTank.h"
#include <cstdlib>
#include <algorithm>
using namespace std;
EnemyTank::EnemyTank(int startX, int startY) {
    moveDelay = 10;
    shootDelay = 5;
    x = startX;
    y = startY;
    rect = { x, y, TILE_SIZE, TILE_SIZE };
    dirX = 0;
    dirY = 1;
    active = true;
}

void EnemyTank::move(const vector<Wall>& walls) {
    if (--moveDelay > 0) return;
    moveDelay = 10;
    int r = rand() % 4;
    if (r == 0) { // lên
        dirX = 0;
        dirY = -10;
    } else if (r == 1) { // xuống
        dirX = 0;
        dirY = 10;
    } else if (r == 2) { // trái
        dirY = 0;
        dirX = -10;
    } else if (r == 3) { // phải
        dirY = 0;
        dirX = 10;
    }
    int newX = x + dirX;
    int newY = y + dirY;
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

void EnemyTank::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}
