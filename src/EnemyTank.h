#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "Bullet.h"
#include "Wall.h"
#include "PlayerTank.h"
#include "Constants.h"
#include <cmath>

using namespace std;
class EnemyTank {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    vector<Bullet> bullets;
    SDL_Texture* texture;
    bool exploding;
    int explodeTimer;
    SDL_Texture* explosionTexture;

    EnemyTank(int startX, int startY);
    void move(const std::vector<Wall>& walls, const std::vector<EnemyTank>& enemies,
              const PlayerTank& player, const PlayerTank& player2);
    void shoot();
    void updateBullets();
    void loadTexture(SDL_Renderer* renderer, const char* path);
    double getAngle() const;
    void render(SDL_Renderer* renderer);
};

#endif
