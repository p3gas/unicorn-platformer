#include "GameManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

GameManager::GameManager(Configuration* configuration)
{
	this->configuration = configuration;
	this->camera.x = configuration->screenWidth / 2;
	this->camera.y = configuration->screenHeight / 2;
	this->camera.w = configuration->screenWidth;
	this->camera.h = configuration->screenHeight;
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
	this->platforms = new SDL_Rect[2];
	this->platforms[0].x = 60;
	this->platforms[0].y = 60;
	this->platforms[0].w = 100;
	this->platforms[0].h = 20;
	this->platforms[1].x = 460;
	this->platforms[1].y = 660;
	this->platforms[1].w = 100;
	this->platforms[1].h = 20;
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

void GameManager::Update(int deltaTime)
{
	this->timer += deltaTime / 1000.f;
	this->player.Update(deltaTime);
	this->camera.x = this->player.GetOriginX() - this->configuration->screenWidth / 2;
	this->camera.y = this->player.GetOriginY() - this->configuration->screenHeight / 2;
	//if (this->camera.x < 0)
	//{
	//	this->camera.x = 0;
	//}
	//if (this->camera.y < 0)
	//{
	//	this->camera.y = 0;
	//}
	printf("x=%d y=%d\n", camera.x, camera.y);
}

void GameManager::Render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(this->renderer);
	SDL_RenderCopy(this->renderer, this->backgroundTexture, NULL, NULL);
	this->player.Draw(this->renderer, &this->camera);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for (int i = 0; i < 2; i++)
	{
		SDL_Rect objectToBeDrew;
		objectToBeDrew.w = this->platforms[i].w;
		objectToBeDrew.h = this->platforms[i].h;
		objectToBeDrew.x = this->platforms[i].x - this->camera.x;
		objectToBeDrew.y = this->platforms[i].y - this->camera.y;
		SDL_RenderDrawRect(this->renderer, &objectToBeDrew);
	}
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

	delete[]this->platforms;

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
