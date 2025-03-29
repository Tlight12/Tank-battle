#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "constants.h"
#include "Wall.h"
#include "PlayerTank.h"
#include "EnemyTank.h"
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

    Game();
    ~Game();
    void run();
    void render();
    void handleEvents();
    void spawnEnemies();
    void generateWalls();
    void update();
};

#endif
