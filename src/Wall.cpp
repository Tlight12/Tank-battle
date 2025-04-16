#include "Wall.h"
#include <SDL_image.h>
using namespace std;
Wall::Wall(int startX, int startY){
    x = startX;
    y = startY;
    active = true;
    texture = nullptr;
    exploding = false;
    explodeTimer = 0;
    explosionTexture = nullptr;
    rect = { x, y, TILE_SIZE, TILE_SIZE };
}

void Wall::loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void Wall::render(SDL_Renderer* renderer) {
    if (exploding) {
        if (explosionTexture) {
            SDL_RenderCopy(renderer, explosionTexture, nullptr, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        return;
    }
    if (active) {
        if (texture) {
            SDL_RenderCopy(renderer, texture, nullptr, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown color
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
