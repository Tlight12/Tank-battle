#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Wall.h"
#include "Constants.h"
#include "Bullet.h"
#include "EnemyTank.h"
#include "PlayerTank.h"
using namespace std;
class Game {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    vector<Wall> walls;
    PlayerTank player;
    PlayerTank player2;
    int enemyNumber;
    vector<EnemyTank> enemies;
    Mix_Music* backgroundMusic;
    Mix_Chunk* explosionSound;
    Mix_Chunk* wall_explosionSound;

    Game();
    void render();
    void handleEvents();
    void spawnEnemies();
    void generateWalls();
    void update();
    void run();
    ~Game();
};

#endif
