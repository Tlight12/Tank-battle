#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include <SDL.h>
#include <vector>
#include "constants.h"
#include "Bullet.h"
#include "Wall.h"
using namespace std;
class PlayerTank {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    vector<Bullet> bullets;
    bool isAlive;

    PlayerTank();
    PlayerTank(int startX, int startY);
    void move(int dx, int dy, const vector<Wall>& walls);
    void shoot();
    void updateBullets();
    void render(SDL_Renderer* renderer);
};

#endif
