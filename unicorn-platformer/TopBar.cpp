#include "TopBar.h"

void TopBar::Init(const Configuration* configuration)
{
	bar.x = 0;
	bar.y = 0;
	bar.w = configuration->screenWidth;
	bar.h = configuration->topBarHight;
}

void TopBar::Render(SDL_Renderer* renderer, float timer, int numberOfLives)
{
	SDL_SetRenderDrawColor(renderer, 150, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &this->bar);
	char textTimer[8];
	sprintf(textTimer, "%05.1f", timer);
	this->textPrinter.Draw(renderer, SDL_Color{255, 255, 255, 255}, textTimer, SDL_Rect{ this->bar.w - 200, 0, 200, 100 });
	SDL_Rect heartLocation = SDL_Rect{ 10, 10, 80, 80 };
	for (int i = 0; i < numberOfLives; i++)
	{
		SDL_RenderCopy(renderer, this->heartTexture, NULL, &heartLocation);
		heartLocation.x += 110;
	}
}

void TopBar::LoadResources(const Configuration* configuration, SDL_Texture* heartTexture)
{
	this->textPrinter.LoadFont(configuration->pathToFont);
	this->heartTexture = heartTexture;
}

void TopBar::Quit()
{
	SDL_DestroyTexture(this->heartTexture);
	this->heartTexture = NULL;
	this->textPrinter.Quit();
}
