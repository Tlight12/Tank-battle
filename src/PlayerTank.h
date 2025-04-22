#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "Bullet.h"
#include "Wall.h"
#include "Constants.h"
using namespace std;
class EnemyTank;

class PlayerTank {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    vector<Bullet> bullets;
    bool isAlive;
    SDL_Texture* texture;
    bool exploding;
    int explodeTimer;
    SDL_Texture* explosionTexture;

    PlayerTank();
    PlayerTank(int startX, int startY);

    void move(int dx, int dy, const vector<Wall>& walls,
              const vector<EnemyTank>& enemies, const PlayerTank& otherPlayer);
    void shoot();
    void updateBullets();
    void loadTexture(SDL_Renderer* renderer, const char* path);
    double getAngle() const;
    void render(SDL_Renderer* renderer);
};

#endif
