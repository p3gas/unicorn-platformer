#include "Obstacle.h"

Obstacle::Obstacle()
{
}

Obstacle::Obstacle(int x, int y, int width, int height, SDL_Texture* texture)
{
	this->body.x = x;
	this->body.y = y;
	this->body.w = width;
	this->body.h = height;
	this->texture = texture;
}

void Obstacle::Draw(SDL_Renderer* renderer, const SDL_Rect* camera)
{
	SDL_Rect objectToBeDrew;
	objectToBeDrew.w = this->body.w;
	objectToBeDrew.h = this->body.h;
	objectToBeDrew.x = this->body.x - camera->x;
	objectToBeDrew.y = this->body.y - camera->y;
	SDL_RenderCopy(renderer, this->texture, NULL, &objectToBeDrew);
}

SDL_Rect Obstacle::GetBody()
{
	return this->body;
}
