#include "Headers/game.h"
#include "Headers/gameObject.h"
#include "Headers/texture.h"
#include "Headers/map.h"

SDL_Rect babeRect2 = { 592,112,48,48 };

void GameObject::SetClips(){
    // right animation
    RunningRight[0].x = 3;
    RunningRight[0].y = 9;
    RunningRight[0].w = 32;
    RunningRight[0].h = 32;

    RunningRight[1].x = 58;
    RunningRight[1].y = 9;
    RunningRight[1].w = 40;
    RunningRight[1].h = 32;

    RunningRight[2].x = 118;
    RunningRight[2].y = 9;
    RunningRight[2].w = 32;
    RunningRight[2].h = 32;

    RunningRight[3].x = 176;
    RunningRight[3].y = 9;
    RunningRight[3].w = 37;
    RunningRight[3].h = 32;

    RunningRight[4].x = 118;
    RunningRight[4].y = 9;
    RunningRight[4].w = 32;
    RunningRight[4].h = 32;
    // left Animation.
    RunningLeft[0].x = 190;
    RunningLeft[0].y = 9;
    RunningLeft[0].w = 32;
    RunningLeft[0].h = 32;

    RunningLeft[1].x = 126;
    RunningLeft[1].y = 9;
    RunningLeft[1].w = 37;
    RunningLeft[1].h = 32;

    RunningLeft[2].x = 74;
    RunningLeft[2].y = 9;
    RunningLeft[2].w = 30;
    RunningLeft[2].h = 32;

    RunningLeft[3].x = 10;
    RunningLeft[3].y = 9;
    RunningLeft[3].w = 35;
    RunningLeft[3].h = 32;

    RunningLeft[4].x = 74;
    RunningLeft[4].y = 9;
    RunningLeft[4].w = 30;
    RunningLeft[4].h = 32;

}

// constructor
GameObject::GameObject(int x, int y){
    //load texture
    objTextureRight = texture::LoadTexture("king_right_2.png");
    objTextureLeft = texture::LoadTexture("king_left_2.png");

    High = Mix_LoadWAV("sound/high.wav");

    xpos = x;
    ypos = y;

    isWin = false;

    //x and y velocity
    xvel = 0;
    yvel = 0;

    //start and jump time variable
    startTime = 0;
    jumpTime = 0;

    frame = 0;
    SetClips();

    //status begin
    status = standing;
    onGround = true;

    // initialize input state
    inputType.left = 3;
    inputType.right = 3;
    inputType.up = 0;
    inputType.jump = 0;

    srcRect.h = KING_HEIGHT;
    srcRect.w = KING_WIDTH;
    srcRect.x = 4;
    srcRect.y = 9;

    collider.h = KING_HEIGHT;
    collider.w = KING_WIDTH;
    collider.x = (int)xpos;
    collider.y = (int)ypos;
}

// returning to initial state.
void GameObject::Reset(){
    // the position of the main character
    xpos = 64;
    ypos = LEVEL_HEIGHT - 100;

    isWin = false;

    xvel = 0;
    yvel = 0;

    startTime = 0;
    jumpTime = 0;

    frame = 0;

    status = standing;
    onGround = true;

    inputType.left = 3;
    inputType.right = 3;
    inputType.up = 0;
    inputType.jump = 0;
}


bool GameObject::checkCollision2(SDL_Rect a, SDL_Rect b)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // A and B 4 peak of the rect
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // check if 2 rect touch the other
    if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB) {
        return false;
    }

    return true;
}

void GameObject::CollideVertical(SDL_Rect& col, SDL_Rect Tile[][60], int Mapping[][60]){

    for (int row = 0; row < 240; row++)
    {
        for (int column = 0; column < 60; column++)
        {
            if (Mapping[row][column] != 3 && checkCollision2(col, Tile[row][column]))
            {
                if (yvel > 0)
                {
                    ypos = Tile[row][column].y - KING_HEIGHT;
                    yvel = 0;
                    onGround = true;
                    //status = standing;
                }
                else if (yvel < 0)
                {
                    ypos = Tile[row][column].y + KING_HEIGHT;
                    yvel = 0;
                    Mix_PlayChannel(-1, High, 0);
                }
                col.y = (int)ypos;
            }
        }
    }
}
void GameObject::CollideHorizontal(SDL_Rect& col, SDL_Rect Tile[][60], int Mapping[][60])
{
    for (int row = 0; row < 240; row++)
    {
        for (int column = 0; column < 60; column++)
        {
            if (Mapping[row][column] != 3 && checkCollision2(col, Tile[row][column]))
            {
                if (xvel > 0)
                {
                    if (onGround == true)
                    {
                        xpos = Tile[row][column].x - KING_WIDTH;
                        xvel = 0;
                        Mix_PlayChannel(-1, High, 0);
                    }
                    else if (onGround == false)
                    {
                        xpos = Tile[row][column].x - KING_WIDTH;
                        xvel = -xvel;
                        if (xvel > -1) xvel = -1;
                        Mix_PlayChannel(-1, High, 0);
                    }
                }
                else if (xvel < 0)
                {
                    if (onGround == true)
                    {
                        xpos = Tile[row][column].x + TILE_WIDTH;
                        xvel = 0;
                        Mix_PlayChannel(-1, High, 0);
                    }
                    else if (onGround == false)
                    {
                        xpos = Tile[row][column].x + TILE_WIDTH;
                        xvel = -xvel;
                        if (xvel < 1) xvel = 1;
                        Mix_PlayChannel(-1, High, 0);
                    }
                }
                col.x = (int)xpos;
            }
        }
    }
}
// functions for moving left-right

void GameObject::RunLeft() {
    status = running;
    xvel = -maxxspeed;
}
void GameObject::RunRight() {
    status = running;
    xvel = maxxspeed;
}
void GameObject::StopRunLeft() {
    xvel = 0;
    status = standing;
}
void GameObject::StopRunRight() {
    xvel = 0;
    status = standing;
}
void GameObject::PrepareJump() {
    startTime = SDL_GetTicks();
    status = charging;
    xvel = 0;
}
void GameObject::Jump() {
    jumpTime = SDL_GetTicks() - startTime;
    status = jumping;
    yvel = -(jumpTime * 0.02);
    if (yvel > -10) yvel = -10;
    if (yvel < -30) yvel = -30;

    onGround = false;

    startTime = 0;
    jumpTime = 0;
}
void GameObject::JumpLeft() {
    jumpTime = SDL_GetTicks() - startTime;
    status = jumping;
    yvel = -(jumpTime * 0.02);
    xvel = -maxxspeed;
    if (yvel > -10) yvel = -10;
    if (yvel < -30) yvel = -30;

    onGround = false;

    startTime = 0;
    jumpTime = 0;
}
void GameObject::JumpRight() {
    jumpTime = SDL_GetTicks() - startTime;
    status = jumping;
    yvel = -(jumpTime * 0.02);
    xvel = maxxspeed;
    if (yvel > -10) yvel = -10;
    if (yvel < -30) yvel = -30;

    onGround = false;

    startTime = 0;
    jumpTime = 0;
}

void GameObject::Update(SDL_Rect Tile[][60], int Mapping[][60]) {
    if (onGround == true && status != charging) {
        if (inputType.right == 1) RunRight();
        if (inputType.left == 1) RunLeft();
        if (inputType.right == 2) StopRunRight();
        if (inputType.left == 2) StopRunLeft();
    }

    if (onGround == true && status == charging) {
        if (inputType.up == 1) inputType.jump = 0;
        if (inputType.right == 1) inputType.jump = 1;
        if (inputType.left == 1) inputType.jump = 2;
    }
    yvel += gravity;
    if (yvel > MAX_FALL_SPEED) yvel = MAX_FALL_SPEED;

    ypos += yvel;
    collider.y = (int)ypos;

    CollideVertical(collider, Tile, Mapping);

    xpos += xvel;
    collider.x = (int)xpos;

    CollideHorizontal(collider, Tile, Mapping);

    //camera follow player
    Camera.y = (int)(ypos + KING_HEIGHT / 2) - SCREEN_HEIGHT / 2;
    if (Camera.y < 0) {
        Camera.y = 0;
    }
    if (Camera.y > LEVEL_HEIGHT - Camera.h) {
        Camera.y = LEVEL_HEIGHT - Camera.h;
    }

    destRect.x = (int)xpos;
    destRect.y = (int)ypos - Camera.y;

    if (checkCollision2(collider, babeRect2) == true) isWin = true;
}

// rendering animation
void GameObject::Render()
{
    SDL_Rect* currentClip;
    if (inputType.right > 0)
    {
        if (status == running)
        {
            frame++;
            if (frame / 15 > 4) frame = 15;
            currentClip = &RunningRight[frame / 15];
        }
        else currentClip = &RunningRight[0];
        SDL_RenderCopy(Game::renderer, objTextureRight, currentClip, &destRect);
    }
    else if (inputType.left > 0)
    {
        if (status == running)
        {
            frame++;
            if (frame / 15 > 4) frame = 15;
            currentClip = &RunningLeft[frame / 15];
        }
        else currentClip = &RunningLeft[0];
        SDL_RenderCopy(Game::renderer, objTextureLeft, currentClip, &destRect);
    }
}

void GameObject::ObjectClose()
{
    SDL_DestroyTexture(objTextureRight);
    SDL_DestroyTexture(objTextureLeft);
    Mix_FreeChunk(High);
    objTextureRight = NULL;
    objTextureLeft = NULL;
    High = NULL;
}

