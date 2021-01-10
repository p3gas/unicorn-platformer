#pragma once
#include <SDL.h>
class Obstacle
{
private:
	SDL_Rect body;
	SDL_Texture* texture = NULL;
public:
	Obstacle();
	Obstacle(int x, int y, int width, int height, SDL_Texture* texture);
	void Draw(SDL_Renderer* renderer, const SDL_Rect* camera);
	SDL_Rect GetBody();
};