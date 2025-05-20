#include "Headers/game.h"
#include "Headers/texture.h"
#include "Headers/gameObject.h"
#include "Headers/map.h"
#include "Headers/textObj.h"

using namespace std;

// texture type
SDL_Texture* background = NULL;
SDL_Texture* foreground = NULL;
SDL_Texture* victory = NULL;
SDL_Texture* bestScore = NULL;
SDL_Texture* babe = NULL;

// mixer and ttf type
Mix_Music* Music = NULL;
TTF_Font* font = NULL;
TTF_Font* fontMenu = NULL;

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

    // create player, map and put startTime to 0 for later use
    player = new GameObject(64, LEVEL_HEIGHT - 100);
    mapper = new Map();
    startTime = 0;
}


void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
    {
        isRunning = false;
    }
    else if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            //if(player->onGround == true)
            {
        case SDLK_RIGHT:
        {
            player->inputType.right = 1;
            player->inputType.left = 0;
            break;
        }
        case SDLK_LEFT:
        {
            player->inputType.left = 1;
            player->inputType.right = 0;
            break;
        }
        case SDLK_UP:
        {
            player->inputType.up = 1;
            break;
        }

        case SDLK_SPACE:
            //if(player->status != jumping)
        {
            player->PrepareJump();
            break;
        }
            }
        }
    }
    else if (event.type == SDL_KEYUP)  //&& event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {
            player->inputType.right = 2;
            break;
        }
        case SDLK_LEFT:
        {
            player->inputType.left = 2;
            break;
        }
        case SDLK_UP:
        {
            player->inputType.up = 2;
            break;
        }
        case SDLK_SPACE:
            //if(player->status != jumping && player->status != charging)
            if (player->onGround == true)
            {
                switch (player->inputType.jump)
                {
                case 0:
                    player->Jump();
                    break;

                case 1:
                    player->JumpRight();
                    break;

                case 2:
                    player->JumpLeft();
                    break;
                }
            }
        }
    }
}

void Game::update() {
    player->Update(mapper->tile, mapper->mapping);
    //babeDestRect.y = babeRect.y - player->Camera.y;
    if (player->isWin == true)
    {
        win = true;
        if (Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(Music, -1);
        }

        // Saving best score to file
        score = timeVal;
        cout << score;
        ifstream input("bestScore.txt");
        input >> hScore;
        ofstream output("bestScore.txt");
        if (score < hScore || hScore == 0) {
            output << score;
        }
        else output << hScore;
    }
    if (player->isWin == false)
    {
        Mix_HaltMusic();
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    mapper->DrawMap(player->Camera);
    texture::Draw(background, player->Camera, BgDest);
    player->Render();
    texture::Draw(foreground, player->Camera, BgDest);
    //texture::Draw(babe, babeSrcRect, babeDestRect);

    //time counting and render on the screen
    string strTime = "TIME: ";
    timeVal = SDL_GetTicks() / 1000 - startTime;
    string timeRes = to_string(timeVal);
    strTime += timeRes;
    timeGame.setText(strTime);
    timeGame.setTextColor(red);
    timeGame.loadFromRenderedText(font, renderer);
    timeGame.renderText(renderer, 25, 15);

    SDL_RenderPresent(renderer);
}

//retry the whole game
void Game::retry()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
    {
        isRunning = false;
    }
    else if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_y:
            win = false;
            player->isWin = false;
            isRetrying = true;
            startTime = SDL_GetTicks()/1000;
            player->Reset();
            mapper = new Map();
            break;
        case SDLK_n:
            isRunning = false;
            break;
        }
    }
    if (score < hScore || hScore == 0) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bestScore, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    else {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, victory, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
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
    player->ObjectClose();
    player = NULL;
    mapper->CloseMap();
    mapper = NULL;

    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    cout << "Game cleaned!" << endl;
}

// menu's function checking whether the option is selected or not
bool Game::checkSelected(const int& x, const int& y, const SDL_Rect& rect)
{
    if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h) return true;
    return false;
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
