#include "Headers/game.h"
#include "Headers/gameObject.h"
#include "Headers/texture.h"
#include "Headers/map.h"

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
    objTextureRight = texture::LoadTexture("image/king_right_2.png");
    objTextureLeft = texture::LoadTexture("image/king_left_2.png");

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
