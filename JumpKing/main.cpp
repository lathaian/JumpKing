#include <iostream>
#include <string>
#include "Headers/header.h"
#include <stdio.h>
#include "Headers/game.h"
#include "Headers/texture.h"

Game* game = nullptr;

int main(int argc, char* args[]) {
    // set up FPS control
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    // initialize game
    game->init("JumpKing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

    // main game loop
    while (game->running()) {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        // delay to maintain consistent frame rate
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    game->clean();
    return 0;
}
