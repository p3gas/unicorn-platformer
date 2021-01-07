#include "TextPrinter.h"

int TextPrinter::LoadFont(const char* pathToFont)
{
	//Open the font
	this->font = TTF_OpenFont(pathToFont, 100);
	if (this->font == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return 1;
	}

	return 0;
}

void TextPrinter::Draw(SDL_Renderer* renderer, SDL_Color textColor, const char* text, SDL_Rect textDimensions)
{
	SDL_Surface* surfaceSymbols = TTF_RenderText_Solid(this->font, text, textColor);
	SDL_Texture* textureSymbols = SDL_CreateTextureFromSurface(renderer, surfaceSymbols);
	SDL_RenderCopy(renderer, textureSymbols, NULL, &textDimensions);

	SDL_FreeSurface(surfaceSymbols);
	SDL_DestroyTexture(textureSymbols);
}

void TextPrinter::Quit()
{
	TTF_CloseFont(this->font);
	this->font = NULL;
}
