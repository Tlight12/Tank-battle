#ifndef WALL_H
#define WALL_H

#include <SDL.h>
#include "Constants.h"
using namespace std;
class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture;
    bool exploding;
    int explodeTimer;
    SDL_Texture* explosionTexture;

    Wall(int startX, int startY);
    void loadTexture(SDL_Renderer* renderer, const char* path);
    void render(SDL_Renderer* renderer);
};

#endif
