#include "Headers/game.h"
#include "Headers/texture.h"
#include "Headers/gameObject.h"
#include "Headers/map.h"
#include "Headers/textObj.h"

using namespace std;

SDL_Texture* background = NULL;
SDL_Texture* foreground = NULL;

// just don't look around this shit is not completed and a complete mess
// just gonna commit today cause deadline is near
// cant even run the game properly but I'm too tired gonna comeback later

Uint32 score, hScore, timeVal, startTime;
// main part of the game including background, player, time and map.
SDL_Rect BgSrc = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }, BgDest = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
GameObject* player;
textObj timeGame;
Map* mapper;



SDL_Renderer* Game::renderer = nullptr;

Game::Game() {}
Game::~Game() {}



// init the game
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flag = 0;
    if (fullscreen) {
        flag = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "Initialized..." << endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flag);
        if (window) {
            cout << "Window created!" << endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            cout << "Renderer created!" << endl;
        }
        // init image
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
            isRunning = false;
        } else {
            cout << "SDL_image initialized!" << endl;
            isRunning = true;
        }
    } else {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        isRunning = false;
    }
    background = texture::LoadTexture("image/background.png");
    foreground = texture::LoadTexture("image/foreground.png");
}



void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update() {
    // too tired to do
}

void Game::render() {
    SDL_RenderClear(renderer);
    texture::Draw(background, player->Camera, BgDest);
    //player->Render();
    texture::Draw(foreground, player->Camera, BgDest);

    // clear the renderer



    //  draw game objects later

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyTexture(background);
    background = NULL;
    SDL_DestroyTexture(foreground);
    foreground = NULL;
    IMG_Quit();
    SDL_Quit();
    cout << "Game cleaned!" << endl;
}

// getters of the bool to run, win or retry the game

bool Game::running()
{
    return isRunning;
}
bool Game::winning()
{
    return win;
}
bool Game::retrying()
{
    return isRetrying;
}
