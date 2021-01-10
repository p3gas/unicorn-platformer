#pragma once
#include "Configuration.h"
#include <SDL.h>
#include <stdio.h>
#include "TextPrinter.h"

class TopBar
{
private:
	SDL_Rect bar;
	TextPrinter textPrinter;
	char* pathToFont;
public:
	void Init(const Configuration* configuration);
	void Render(SDL_Renderer* renderer, float timer, int numberOfLives);
	void LoadResources(const Configuration* configuration);
	void Quit();
};

