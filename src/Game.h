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
    TTF_Font* font;
    bool gameOver;
    bool playerWon;
    bool menu;
    int menuSelection;
    bool singlePlayerMode;

    Game();
    void render();
    void renderMenu();
    void renderTextColor(const char* message, int x, int y, int size, bool center, SDL_Color color);
    void renderText(const char* message, int x, int y, int size, bool center);
    void handleEvents();
    void spawnEnemies();
    void generateWalls();
    void update();
    void resetGame();
    void run();
    ~Game();
};

#endif
