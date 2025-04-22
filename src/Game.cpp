#include "Game.h"
#include "Constants.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
using namespace std;
Game::Game(){
    enemyNumber = 7;
    running = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }
    window = SDL_CreateWindow("Tank Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        cerr << "SDL_image could not initialize! SDL_Error: " << IMG_GetError() << endl;
        running = false;
    }
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf could not initialize! SDL_Error: " << TTF_GetError() << endl;
        running = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! SDL_Error: " << Mix_GetError() << endl;
        running = false;
    }
    backgroundMusic = Mix_LoadMUS("sound/music.mp3");
    if (backgroundMusic) {
        Mix_PlayMusic(backgroundMusic, -1);
    } else {
        cerr << "Failed to load background music! SDL_Error: " << Mix_GetError() << endl;
    }
    explosionSound = Mix_LoadWAV("sound/explosion.wav");
    if (!explosionSound) {
        cerr << "Failed to load explosion sound! SDL_Error: " << Mix_GetError() << endl;
    }
    wall_explosionSound = Mix_LoadWAV("sound/wall_explosion.wav");
    if (!wall_explosionSound) {
        cerr << "Failed to load wall explosion sound! SDL_Error: " << Mix_GetError() << endl;
    }
    generateWalls();
    for (auto& wall : walls) {
        wall.loadTexture(renderer, "img/wall.png");
        wall.explosionTexture = IMG_LoadTexture(renderer, "img/wall_explosion.png");
    }
    player = PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
    player2 = PlayerTank(((MAP_WIDTH - 5) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
    player.loadTexture(renderer, "img/1.png");
    player2.loadTexture(renderer, "img/2.png");
    player.explosionTexture = IMG_LoadTexture(renderer, "img/explosion.png");
    player2.explosionTexture = IMG_LoadTexture(renderer, "img/explosion.png");
    spawnEnemies();
    for (auto& enemy : enemies) {
        enemy.loadTexture(renderer, "img/enemy.png");
        enemy.explosionTexture = IMG_LoadTexture(renderer, "img/explosion.png");
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < MAP_HEIGHT - 1; ++i) {
        for (int j = 1; j < MAP_WIDTH - 1; ++j) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &tile);
        }
    }
    for (size_t i = 0; i < walls.size(); i++) {
        walls[i].render(renderer);
    }
    player.render(renderer);
    player2.render(renderer);
    for (auto &enemy : enemies) {
        enemy.render(renderer);
    }
    SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                // Player 1 controls
                case SDLK_UP:    player.move(0, -10, walls, enemies, player2); break;
                case SDLK_DOWN:  player.move(0, 10, walls, enemies, player2); break;
                case SDLK_LEFT:  player.move(-10, 0, walls, enemies, player2); break;
                case SDLK_RIGHT: player.move(10, 0, walls, enemies, player2); break;
                case SDLK_KP_6:  player.shoot(); break;
                // Player 2 controls
                case SDLK_w:     player2.move(0, -10, walls, enemies, player); break;
                case SDLK_s:     player2.move(0, 10, walls, enemies, player); break;
                case SDLK_a:     player2.move(-10, 0, walls, enemies, player); break;
                case SDLK_d:     player2.move(10, 0, walls, enemies, player); break;
                case SDLK_SPACE: player2.shoot(); break;
                // Escape key to quit
                case SDLK_ESCAPE: running = false; break;
            }
        }
    }
}

void Game::spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;
            for (const auto& wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    break;
                }
            }
        }
        enemies.push_back(EnemyTank(ex, ey));
    }
}

void Game::generateWalls() {
    for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
        for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
            walls.push_back(Wall(j * TILE_SIZE, i * TILE_SIZE));
        }
    }
}

void Game::update() {
    player.updateBullets();
    player2.updateBullets();

    for (auto& enemy : enemies) {
        enemy.move(walls, enemies, player, player2);
        enemy.updateBullets();
        if (rand() % 100 < 2)
            enemy.shoot();
    }

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    wall.exploding = true;
                    wall.explodeTimer = 15;
                    bullet.active = false;
                    if (wall_explosionSound){
                        Mix_PlayChannel(-1, wall_explosionSound, 0);
                    }
                    break;
                }
            }
        }
    }
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            if (player.isAlive && SDL_HasIntersection(&bullet.rect, &player.rect)) {
                player.exploding = true;
                player.explodeTimer = 20;
                player.isAlive = false;
                bullet.active = false;
                if (explosionSound){
                    Mix_PlayChannel(-1, explosionSound, 0);
                }
            }
            if (player2.isAlive && SDL_HasIntersection(&bullet.rect, &player2.rect)) {
                player2.exploding = true;
                player2.explodeTimer = 20;
                player2.isAlive = false;
                bullet.active = false;
                if (explosionSound){
                    Mix_PlayChannel(-1, explosionSound, 0);
                }
            }
        }
    }
    for (auto& bullet : player.bullets) {
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                wall.exploding = true;
                wall.explodeTimer = 15;
                bullet.active = false;
                if (wall_explosionSound){
                    Mix_PlayChannel(-1, wall_explosionSound, 0);
                }
                break;
            }
        }
    }
    for (auto& bullet : player.bullets) {
        for (auto& enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                bullet.active = false;
                enemy.exploding = true;
                enemy.explodeTimer = 20;
                enemy.active = false;
                if (explosionSound){
                    Mix_PlayChannel(-1, explosionSound, 0);
                }
            }
        }
    }
    for (auto& bullet : player.bullets) {
          if (player2.isAlive && SDL_HasIntersection(&bullet.rect, &player2.rect)) {
              bullet.active = false;
              player2.exploding = true;
              player2.explodeTimer = 20;
              player2.isAlive = false;
              if (explosionSound) Mix_PlayChannel(-1, explosionSound, 0);
          }
      }
    for (auto& bullet : player2.bullets) {
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                wall.exploding = true;
                wall.explodeTimer = 15;
                bullet.active = false;
                if (wall_explosionSound){
                    Mix_PlayChannel(-1, wall_explosionSound, 0);
                }
                break;
            }
        }
    }
    for (auto& bullet : player2.bullets) {
        for (auto& enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                bullet.active = false;
                enemy.exploding = true;
                enemy.explodeTimer = 20;
                enemy.active = false;
                if (explosionSound){
                    Mix_PlayChannel(-1, explosionSound, 0);
                }
            }
        }
    }
    for (auto& bullet : player2.bullets) {
        if (player.isAlive && SDL_HasIntersection(&bullet.rect, &player.rect)) {
            bullet.active = false;
            player.exploding = true;
            player.explodeTimer = 20;
            player.isAlive = false;
            if (explosionSound) Mix_PlayChannel(-1, explosionSound, 0);
        }
    }


    enemies.erase(remove_if(enemies.begin(), enemies.end(),
        [] (EnemyTank &e) { return !e.active && !e.exploding; }), enemies.end());

    if (enemies.empty()) {
        running = false;
    }

    if (!player.isAlive && !player2.isAlive) {
        running = false;
        return;
    }
    if (player.exploding) {
        player.explodeTimer--;
        if (player.explodeTimer <= 0) {
            player.exploding = false;
            player.isAlive = false;
        }
    }
    if (player2.exploding) {
        player2.explodeTimer--;
        if (player2.explodeTimer <= 0) {
            player2.exploding = false;
            player2.isAlive = false;
        }
    }
    for (auto& enemy : enemies) {
        if (enemy.exploding) {
            enemy.explodeTimer--;
            if (enemy.explodeTimer <= 0) {
                enemy.exploding = false;
                enemy.active = false;
            }
        }
    }
    for (auto& wall : walls) {
        if (wall.exploding) {
            wall.explodeTimer--;
            if (wall.explodeTimer <= 0) {
                wall.exploding = false;
            }
        }
    }
}

void Game::run() {
    Uint32 frameStart;
    int frameTime;
    const int FRAME_DELAY = 1000 / 60;

    while (running) {
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
}

Game::~Game() {
    if (player.texture) SDL_DestroyTexture(player.texture);
    if (player2.texture) SDL_DestroyTexture(player2.texture);
    if (player.explosionTexture) SDL_DestroyTexture(player.explosionTexture);
    if (player2.explosionTexture) SDL_DestroyTexture(player2.explosionTexture);
    for (auto& enemy : enemies) {
        if (enemy.texture) SDL_DestroyTexture(enemy.texture);
        if (enemy.explosionTexture) SDL_DestroyTexture(enemy.explosionTexture);
    }
    for (auto& wall : walls) {
        if (wall.texture) SDL_DestroyTexture(wall.texture);
        if (wall.explosionTexture) SDL_DestroyTexture(wall.explosionTexture);
    }
    if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
    if (explosionSound) Mix_FreeChunk(explosionSound);
    if (wall_explosionSound) Mix_FreeChunk(wall_explosionSound);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
