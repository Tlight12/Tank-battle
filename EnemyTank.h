#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <SDL.h>
#include <vector>
#include "constants.h"
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

    EnemyTank(int startX, int startY);
    void move(const vector<Wall>& walls);
    void shoot();
    void updateBullets();
    void render(SDL_Renderer* renderer);
};

#endif
