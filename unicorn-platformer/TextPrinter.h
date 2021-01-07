#pragma once
#include <SDL_ttf.h>
#include <stdio.h>

class TextPrinter
{
private:
	TTF_Font* font = NULL;
public:
	int LoadFont(const char* pathToFont);
	void Draw(SDL_Renderer* renderer, SDL_Color textColor, const char* text, SDL_Rect textDimensions);
	void Quit();
};

