#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "Configuration.h"
#include "Player.h"
#include "World.h"
#include "TextPrinter.h"

class GameManager
{
private:
	SDL_Event inputEvent;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	Configuration* configuration;
	SDL_Texture* backgroundTexture = NULL;
	Player player;
	SDL_Rect camera;
	World world;
	TextPrinter textPrinter;
	bool isRunning = false;
	float timer = 0.0f;
public:
	GameManager(Configuration* configuration);
	int Init();
	int LoadResources();
	void ProcessInput();
	void Update(int deltaTime);
	void Render();
	void Quit();
	bool IsRunning();
	void RestartGame();
private:
	SDL_Texture* LoadTexture(const char* path);
};

