#include "GameManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

GameManager::GameManager(Configuration* configuration)
{
	this->configuration = configuration;
}

int GameManager::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}
	this->window = SDL_CreateWindow("Unicorn the Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->configuration->screenWidth,
		this->configuration->screenHeight, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return 2;
	}
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	if (this->renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return 3;
	}
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return 4;
	}
	this->isRunning = true;

	return 0;
}

int GameManager::LoadResources()
{
	this->backgroundTexture = this->LoadTexture(this->configuration->backgroundTexturePath);
	if (this->backgroundTexture == NULL)
	{
		printf("Unable to load texture from: %s\n", this->configuration->backgroundTexturePath);
		return 10;
	}
	return 0;
}

void GameManager::ProcessInput()
{
	while (SDL_PollEvent(&this->inputEvent) != 0)
	{
		if (this->inputEvent.type == SDL_QUIT)
		{
			this->isRunning = false;
		}
		else if (this->inputEvent.type == SDL_KEYDOWN)
		{
			switch (this->inputEvent.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				this->isRunning = false;
				break;
			case SDLK_n:
				printf("TODO: NEW GAME SHOULD BE STARTED\n");
				break;
			}
		}
	}
}

void GameManager::Update(float deltaTime)
{
	this->timer += deltaTime;
	printf("%f\n", this->timer);
}

void GameManager::Render()
{
	SDL_RenderClear(this->renderer);
	SDL_RenderCopy(this->renderer, this->backgroundTexture, NULL, NULL);
	SDL_RenderPresent(this->renderer);
}

void GameManager::Quit()
{
	SDL_DestroyTexture(this->backgroundTexture);
	this->backgroundTexture = NULL;
	SDL_DestroyWindow(this->window);
	this->window = NULL;
	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

bool GameManager::IsRunning()
{
	return this->isRunning;
}

SDL_Texture* GameManager::LoadTexture(const char* path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
		return NULL;
	}
	newTexture = SDL_CreateTextureFromSurface(this->renderer, loadedSurface);
	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
	}
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}
