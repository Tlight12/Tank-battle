#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;
Game::Game() {
    running = true;
    enemyNumber = 5;
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
    generateWalls();
    player = PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
    player2 = PlayerTank(((MAP_WIDTH - 5) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
    spawnEnemies();
    srand(static_cast<unsigned int>(time(0))); // khởi tạo seed random
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);

    // Vẽ nền (tạo các ô vuông)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < MAP_HEIGHT - 1; ++i) {
        for (int j = 1; j < MAP_WIDTH - 1; ++j) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &tile);
        }
    }
    for (auto &wall : walls) {
        wall.render(renderer);
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
                // Điều khiển cho Player 1
                case SDLK_UP:    player.move(0, -10, walls); break;
                case SDLK_DOWN:  player.move(0, 10, walls); break;
                case SDLK_LEFT:  player.move(-10, 0, walls); break;
                case SDLK_RIGHT: player.move(10, 0, walls); break;
                case SDLK_KP_6:  player.shoot(); break;
                // Điều khiển cho Player 2
                case SDLK_w:     player2.move(0, -10, walls); break;
                case SDLK_s:     player2.move(0, 10, walls); break;
                case SDLK_a:     player2.move(-10, 0, walls); break;
                case SDLK_d:     player2.move(10, 0, walls); break;
                case SDLK_SPACE: player2.shoot(); break;
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

    for (auto &enemy : enemies) {
        enemy.move(walls);
        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot();
        }
    }

    // Xử lý va chạm của đạn với tường và đối tượng khác
    for (auto &enemy : enemies) {
        for (auto &bullet : enemy.bullets) {
            for (auto &wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }
    }

    for (auto &bullet : player.bullets) {
        for (auto &wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                bullet.active = false;
                break;
            }
        }
    }
    for (auto &bullet : player.bullets) {
        for (auto &enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                enemy.active = false;
                bullet.active = false;
            }
        }
    }
    for (auto &bullet : player2.bullets) {
        for (auto &wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                bullet.active = false;
                break;
            }
        }
    }
    for (auto &bullet : player2.bullets) {
        for (auto &enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                enemy.active = false;
                bullet.active = false;
            }
        }
    }

    enemies.erase(remove_if(enemies.begin(), enemies.end(),
                                  [](EnemyTank &e) { return !e.active; }), enemies.end());

    for (auto &enemy : enemies) {
        for (auto &bullet : enemy.bullets) {
            if (player.isAlive && SDL_HasIntersection(&bullet.rect, &player.rect)) {
                player.isAlive = false;
                bullet.active = false;
            }
            if (player2.isAlive && SDL_HasIntersection(&bullet.rect, &player2.rect)) {
                player2.isAlive = false;
                bullet.active = false;
            }
        }
    }

    if (!player.isAlive && !player2.isAlive)
        running = false;
    if (enemies.empty())
        running = false;
}

void Game::run() {
    Uint32 frameStart;
    int frameTime;
    const int FRAME_DELAY = 1000 / 60; // 60 FPS
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
