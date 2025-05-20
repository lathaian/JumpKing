#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Game.h"
#include "header.h"
#include <math.h>

#define gravity 0.8
#define MAX_FALL_SPEED 20

using namespace std;

//condition of the player
enum condition { standing, running, charging, jumping };

//input key
typedef struct {
    int left;
    int right;
    int up;
    int jump;
}
Input;

class GameObject
{
private:
    // position x and y
    double xpos;
    double ypos;

    // velocity x and y
    double xvel;
    double yvel;

    int frame;

    Uint32 startTime;
    int jumpTime;

     //texture and stuff
    SDL_Texture* objTextureRight = NULL;
    SDL_Texture* objTextureLeft = NULL;
    SDL_Rect srcRect, destRect, collider;

    // sound
    Mix_Chunk* High = NULL;

    // for running animation
    SDL_Rect RunningRight[5];
    SDL_Rect RunningLeft[5];

    // state tracking
    condition status;
    Input inputType;
    bool onGround;
    bool isWin;

public:
    // constructor and destructor
    GameObject(int x, int y);
    ~GameObject();

    //  to access private members
    friend void Game::handleEvents();
    friend void Game::update();
    friend void Game::retry();

    // get the collider
    SDL_Rect GetCollider() { return collider; }

    // update function
    void Update(SDL_Rect Tile[][60], int Mapping[][60]);

    // run function
    void RunLeft();
    void RunRight();

    // jump function
    void PrepareJump();
    void Jump();
    void JumpLeft();
    void JumpRight();

    // stop action function
    void StopRunRight();
    void StopRunLeft();
    void StopRunUp();
    void StopRunDown();

    // reset function
    void Reset();

    // set clips and render, close function
    void SetClips();
    void Render();
    void ObjectClose();

    // collide function and check collisions
    void CollideVertical(SDL_Rect& col, SDL_Rect Tile[][60], int Mapping[][60]);
    void CollideHorizontal(SDL_Rect& col, SDL_Rect Tile[][60], int Mapping[][60]);
    bool checkCollision2(SDL_Rect a, SDL_Rect b);

     // get speed and velocity
    int Getxspeed() { return xspeed; }
    int Getxvel() { return (int)xvel; }
    int Getyvel() { return (int)yvel; }

    // speed stat
    static const int xspeed = 1;
    static const int yspeed = 1;

    // limit speed stat
    static const int maxxspeed = 6;
    static const int maxyspeed = 8;

    // character stat
    static const int KING_WIDTH = 48;
    static const int KING_HEIGHT = 48;

    // camera that follows the character position
    SDL_Rect Camera = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

};

#endif // GAMEOBJECT_H
