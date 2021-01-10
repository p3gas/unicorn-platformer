#include "GameManager.h"

GameManager::GameManager(Configuration* configuration)
{
	this->configuration = configuration;
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
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return 5;
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
	this->world.Build();
	this->textPrinter.LoadFont(this->configuration->pathToFont);
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
				this->RestartGame();
				break;
			case SDLK_d:
				this->player.ChangeControlMode();
				break;
			}
		}
	}
}

void GameManager::Update(int deltaTime)
{
	this->timer += deltaTime / 1000.f;
	this->player.Update(deltaTime);

	if (this->world.AdjustPlayerPosition(&this->player) == PLAYER_DEATH)
	{
		this->RestartGame();
	}
	if (this->world.IsCollidingWithObstacle(&this->player))
	{
		this->RestartGame();
	}

	this->camera.x = this->player.GetOriginX() - this->configuration->screenWidth / 5;
	this->camera.y = this->player.GetOriginY() - this->configuration->screenHeight / 2;
	if (this->camera.y < -this->configuration->topBarHight)
	{
		this->camera.y = -this->configuration->topBarHight;
	}
}

void GameManager::Render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(this->renderer);
	SDL_RenderCopy(this->renderer, this->backgroundTexture, NULL, NULL);
	this->player.Draw(this->renderer, &this->camera);
	SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	this->world.Draw(this->renderer, &this->camera);
	
	SDL_Rect topBar;
	topBar.x = 0;
	topBar.y = 0;
	topBar.w = this->configuration->screenWidth;
	topBar.h = this->configuration->topBarHight;
	SDL_SetRenderDrawColor(this->renderer, 150, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(this->renderer, &topBar);
	char textTimer[8];
	sprintf(textTimer, "%05.1f", this->timer);
	this->textPrinter.Draw(this->renderer, SDL_Color{255, 255, 255, 255}, textTimer, SDL_Rect{ this->configuration->screenWidth - 200, 0, 200, 100 });

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

	this->textPrinter.Quit();
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
