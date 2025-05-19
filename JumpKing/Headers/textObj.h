#pragma once
#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "game.h"
#include <string>
#include <stdio.h>
#include <SDL_ttf.h>

using namespace std;
// enum to set the colors
enum textColors{
	white = 0,
	black = 1,
	red = 2,
	yellow = 3,
	green = 4,
	blue = 5,
};

class textObj
{
public:
	// constructor and destructor
	textObj();
	~textObj();

	// text related function to draw, erase and stuffs
	bool loadFromRenderedText(TTF_Font* font, SDL_Renderer* screen);
	void free();
	void renderText(SDL_Renderer* screen, int x, int y,
		SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL,
		SDL_RendererFlip flip = SDL_FLIP_NONE);
	void setText(const string& text) { strVal = text; }
	void setTextColor(const int& color);
private:
	// variables
	string strVal;
	SDL_Color textColor;
	SDL_Texture* mTexture;

	//text width and height
	int mWidth;
	int mHeight;
};

#endif // TEXT_OBJECT_H
