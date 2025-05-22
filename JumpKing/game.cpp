#include "Headers/game.h"
#include "Headers/texture.h"
#include "Headers/gameObject.h"
#include "Headers/map.h"
#include "Headers/textObj.h"

using namespace std;

// texture type
SDL_Texture* imgStart = NULL;
SDL_Texture* background = NULL;
SDL_Texture* foreground = NULL;
SDL_Texture* victory = NULL;
SDL_Texture* bestScore = NULL;
SDL_Texture* babe = NULL;

SDL_Rect babeSrcRect = { 0,0,48,48 };
SDL_Rect babeRect = { 592,112,48,48 };
SDL_Rect babeDestRect = { 592,112,48,48 };

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


int Game::createMenu(TTF_Font* font){
    // load menu image
    if (font == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Font parameter is null");
        return 1;
    }
     if (Game::renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer is null");
        return 1;
     }
    imgStart = texture::LoadTexture("image/main_menu.png");
    if (imgStart == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load menu image: %s", SDL_GetError());
        return 1;
    }
    texture::Draw(imgStart, player->Camera, BgDest);

    // 0 = start, 1 = exit;
    const int numMenu = 2;

    // rect to put the text of the menu in, and also helps check the selected or not
    SDL_Rect menuRect[numMenu];

    menuRect[0].x = 480;
    menuRect[0].y = 360;
    menuRect[0].w = 960;
    menuRect[0].h = 40;

    menuRect[1].x = 480;
    menuRect[1].y = 400;
    menuRect[1].w = 960;
    menuRect[1].h = 40;

    // contain the text, gonna be the src rect for render purpose
    textObj textMenu[numMenu];

    textMenu[0].setText("New Game");
    textMenu[0].setTextColor(white);

    textMenu[1].setText("Exit");
    textMenu[1].setTextColor(white);

    // bool array that check whether the text is selected or not
    bool chosen[numMenu] = {false, false};

    // menu event handing, turn red when the mouse point to the text
    SDL_Event mouseEvent;
    while(true){
        texture::Draw(imgStart, player->Camera, BgDest);
        for (int i=0; i < numMenu; i++){
            // render the text
            textMenu[i].loadFromRenderedText(font, renderer);
            textMenu[i].renderText(renderer, menuRect[i].x, menuRect[i].y);
        }
        while(SDL_PollEvent(&mouseEvent)){
            switch(mouseEvent.type){
                case SDL_QUIT:
                    return 1;
                case SDL_MOUSEMOTION:
                    for (int i=0; i<numMenu; i++){
                        if(checkSelected(mouseEvent.motion.x, mouseEvent.motion.y, menuRect[i])){
                            if (chosen[i] == false){
                                chosen[i] = true;
                                textMenu[i].setTextColor(red);
                            }
                        } else {
                            if (chosen[i] == true){
                                chosen[i] = false;
                                textMenu[i].setTextColor(white);
                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    for (int i=0; i < numMenu; i++){
                        if(checkSelected(mouseEvent.motion.x, mouseEvent.motion.y, menuRect[i])){
                                return i;
                            }
                        }
                    break;
                case SDL_KEYDOWN:
                    if (mouseEvent.key.keysym.sym == SDLK_ESCAPE){
                        return 1;
                    }
                default:
                    break;
            };
        }
        SDL_RenderPresent(renderer);
    }

    return 0;
}

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
        // time font (smaller size)
        if (TTF_Init() == 0) {
            font = TTF_OpenFont("font/font2.ttf", 24);
        }

        // menu font (just bigger size)
        if(TTF_Init() == 0){
            fontMenu = TTF_OpenFont("font/font2.ttf", 32);
        }
    } else {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        isRunning = false;
    }

    // mixer for sfx of the game
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    // win sound
    Music = Mix_LoadMUS("sound/win.wav");

    background = texture::LoadTexture("image/background.png");
    foreground = texture::LoadTexture("image/foreground.png");
    babe = texture::LoadTexture("image/babe.png");
    victory = texture::LoadTexture("image/victory.png");
    bestScore = texture::LoadTexture("image/highScore.png");

    // icon loading.
    string icoName = "image/icon.bmp";
    SDL_Surface* loadedSurface = SDL_LoadBMP(icoName.c_str());
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));
    SDL_SetWindowIcon(window, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    // create player, map and put startTime to 0 for later use
    player = new GameObject(64, LEVEL_HEIGHT - 100);
    mapper = new Map();
    startTime = 0;

    int menuCheck = createMenu(fontMenu);
    if (menuCheck == 0){
        isRunning = true;
        startTime = SDL_GetTicks()/1000;
    } else if (menuCheck == 1){
        isRunning = false;
    } else {
        isRunning = false;
    }

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
    babeDestRect.y = babeRect.y - player->Camera.y;
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
    texture::Draw(babe, babeSrcRect, babeDestRect);

    //time counting and render on the screen
    string strTime = "TIME: ";
    timeVal = SDL_GetTicks() / 1000 - startTime;
    string timeRes = to_string(timeVal);
    strTime += timeRes;
    timeGame.setText(strTime);
    timeGame.setTextColor(white);
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
    SDL_DestroyTexture(imgStart);
    imgStart = NULL;
    SDL_DestroyTexture(background);
    background = NULL;
    SDL_DestroyTexture(foreground);
    foreground = NULL;
    SDL_DestroyTexture(babe);
    babe = NULL;
    SDL_DestroyTexture(victory);
    victory = NULL;
    SDL_DestroyTexture(bestScore);
    bestScore = NULL;
    player->ObjectClose();
    player = NULL;
    mapper->CloseMap();
    mapper = NULL;
    TTF_CloseFont(font);
    font = NULL;

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
