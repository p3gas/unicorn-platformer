#pragma once
#include <SDL.h>
#include "Configuration.h"

class GameManager
{
private:
	SDL_Event inputEvent;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	Configuration* configuration;
	SDL_Texture* backgroundTexture = NULL;
	bool isRunning = false;
	float timer = 0.0f;
public:
	GameManager(Configuration* configuration);
	int Init();
	int LoadResources();
	void ProcessInput();
	void Update(float deltaTime);
	void Render();
	void Quit();
	bool IsRunning();
private:
	SDL_Texture* LoadTexture(const char* path);
};

