#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "Bullet.h"
#include "Wall.h"
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
    void move(const vector<Wall>& walls);
    void shoot();
    void updateBullets();
    void loadTexture(SDL_Renderer* renderer, const char* path);
    double getAngle() const;
    void render(SDL_Renderer* renderer);
};

#endif
