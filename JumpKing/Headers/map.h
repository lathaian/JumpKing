#pragma once
#ifndef MAP_H
#define MAP_H

#include "Header.h"
#include "Game.h"
#include "GameObject.h"

class Map {
private:
    SDL_Rect src, dest;

    SDL_Texture* dirt = NULL;
    SDL_Texture* grass = NULL;
    SDL_Texture* red_grass = NULL;
    SDL_Texture* stone = NULL;

    // map data
    int mapping[240][60];
    SDL_Rect tile[240][60];

public:
     Map();
    ~Map();
    // function to load, draw and close the map
    void LoadMap(int arr[240][60]);
    void DrawMap(SDL_Rect Camera);
    void DrawBabe(SDL_Rect Camera);
    void CloseMap();

    // collision checking function
    bool checkCollision(SDL_Rect a, SDL_Rect b);

    // friend function
    friend void GameObject::Update(SDL_Rect Tile[][60], int Mapping[][60]);
    friend bool Collide(SDL_Rect Tile[], int Mapping[][60]);
    friend void Game::update();
};

#endif // MAP_H
