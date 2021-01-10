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
	char lives[3];
	sprintf(lives, "%d", numberOfLives);
	this->textPrinter.Draw(renderer, SDL_Color{255, 255, 255, 255}, lives, SDL_Rect{ 0, 0, 50, 100 });
}

void TopBar::LoadResources(const Configuration* configuration)
{
	this->textPrinter.LoadFont(configuration->pathToFont);
}

void TopBar::Quit()
{
	this->textPrinter.Quit();
}
