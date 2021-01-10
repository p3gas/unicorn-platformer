#include "GameManager.h"

GameManager::GameManager(Configuration* configuration)
{
	this->configuration = configuration;
	this->camera.w = configuration->screenWidth;
	this->camera.h = configuration->screenHeight;
	this->gameState = MENU;
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
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return 5;
	}
	this->topBar.Init(this->configuration);
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
	this->menuTexture = this->LoadTexture(this->configuration->menuTexturePath);
	if (this->menuTexture == NULL)
	{
		printf("Unable to load texture from: %s\n", this->configuration->menuTexturePath);
		return 11;
	}
	this->world.Build(this->LoadTexture(this->configuration->starTexturePath), this->LoadTexture(this->configuration->stalactiteTexturePath));
	this->topBar.LoadResources(this->configuration);
	this->player.SetTexture(this->LoadTexture(this->configuration->playerTexturePath));
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
				if (this->gameState == GAME)
				{
					this->RestartGame();
					this->player.ResetLives();
				}
				break;
			case SDLK_d:
				if (this->gameState == GAME)
				{
					this->player.ChangeControlMode();
				}
				break;
			case SDLK_SPACE:
				if (this->gameState == MENU)
				{
					this->RestartGame();
					this->player.ResetLives();
					this->gameState = GAME;
				}
				break;
			}			
		}
	}
}

void GameManager::Update(int deltaTime)
{
	if (this->gameState == GAME)
	{
		this->timer += deltaTime / 1000.f;
		this->player.Update(deltaTime);

		if (this->world.AdjustPlayerPosition(&this->player) == PLAYER_DEATH)
		{
			if (this->player.Dies())
			{
				this->gameState = MENU;
			}
			this->RestartGame();
		}
		if (!this->player.IsDashing() && this->world.IsCollidingWithStar(&this->player))
		{
			if (this->player.Dies())
			{
				this->gameState = MENU;
			}
			this->RestartGame();
		}
		if (this->world.IsCollidingWithStalactite(&this->player))
		{
			if (this->player.Dies())
			{
				this->gameState = MENU;
			}
			this->RestartGame();
		}

		this->camera.x = this->player.GetOriginX() - this->configuration->screenWidth / 8;
		this->camera.y = this->player.GetOriginY() - this->configuration->screenHeight / 2;
		if (this->camera.y < -this->configuration->topBarHight)
		{
			this->camera.y = -this->configuration->topBarHight;
		}
	}
}

void GameManager::Render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(this->renderer);

	if (this->gameState == GAME)
	{		
		SDL_RenderCopy(this->renderer, this->backgroundTexture, NULL, NULL);
		this->player.Draw(this->renderer, &this->camera);
		SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		this->world.Draw(this->renderer, &this->camera);
		this->topBar.Render(this->renderer, this->timer, this->player.GetNumberOfLives());
	}
	else if (this->gameState == MENU)
	{
		SDL_RenderCopy(this->renderer, this->menuTexture, NULL, NULL);
	}

	SDL_RenderPresent(this->renderer);
}

void GameManager::Quit()
{
	SDL_DestroyTexture(this->backgroundTexture);
	this->backgroundTexture = NULL;
	SDL_DestroyTexture(this->menuTexture);
	this->menuTexture = NULL;
	SDL_DestroyWindow(this->window);
	this->window = NULL;
	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	this->topBar.Quit();
	this->player.Destroy();
	this->world.Destroy();

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool GameManager::IsRunning()
{
	return this->isRunning;
}

void GameManager::RestartGame()
{
	this->timer = 0.0f;
	this->player.SetPosition(this->world.GetStartPosition().x, this->world.GetStartPosition().y - this->player.GetBody().h);
	this->player.Reset();
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
