#include "World.h"
#include <stdio.h>

void World::Build()
{
	this->numberOfPlatforms = 3;
	this->levelWidth = 5040;
	this->platforms = new SDL_Rect[this->numberOfPlatforms];
	this->platforms[0] = this->CreateRect(0, 700, 5040, 80);
	this->platforms[1] = this->CreateRect(5040, 700, 1280, 80);
	this->platforms[2] = this->CreateRect(-256, 700, 256, 80);

	this->numberOfObstacles = 6;
	this->obstacles = new SDL_Rect[this->numberOfObstacles];
	this->obstacles[0] = this->CreateRect(800, 600, 100, 100);
	this->obstacles[1] = this->CreateRect(1900, 600, 100, 100);
	this->obstacles[2] = this->CreateRect(3100, 600, 100, 100);
	this->obstacles[3] = this->CreateRect(4000, 600, 100, 100);
	this->obstacles[4] = this->CreateRect(4700, 600, 100, 100);
	this->obstacles[5] = this->CreateRect(5840, 600, 100, 100);
}

void World::Draw(SDL_Renderer* renderer, const SDL_Rect* camera)
{
	for (int i = 0; i < this->numberOfPlatforms; i++)
	{
		SDL_Rect objectToBeDrew;
		objectToBeDrew.w = this->platforms[i].w;
		objectToBeDrew.h = this->platforms[i].h;
		objectToBeDrew.x = this->platforms[i].x - camera->x;
		objectToBeDrew.y = this->platforms[i].y - camera->y;
		SDL_SetRenderDrawColor(renderer, 102, 204, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &objectToBeDrew);
	}
	for (int i = 0; i < this->numberOfObstacles; i++)
	{
		SDL_Rect objectToBeDrew;
		objectToBeDrew.w = this->obstacles[i].w;
		objectToBeDrew.h = this->obstacles[i].h;
		objectToBeDrew.x = this->obstacles[i].x - camera->x;
		objectToBeDrew.y = this->obstacles[i].y - camera->y;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &objectToBeDrew);
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(renderer, &objectToBeDrew);
	}
}

int World::GetLevelWidth()
{
    return this->levelWidth;
}

int World::GetLevelHeight()
{
    return this->levelHeight;
}

void World::Destroy()
{
	delete[]this->platforms;
	delete[]this->obstacles;
}

void World::AdjustPlayerPosition(Player* player)
{
	for (int i = 0; i < this->numberOfPlatforms; i++)
	{
		if (this->IsColliding(player->GetBody(), this->platforms[i]))
		{
			player->SetPosition(player->GetBody().x, this->platforms[i].y - player->GetBody().h);
		}
	}
	if (player->GetBody().x > this->levelWidth)
	{
		player->SetPosition(this->levelWidth - player->GetBody().x, player->GetBody().y);
	}
}

bool World::IsCollidingWithObstacle(Player* player)
{
	for (int i = 0; i < this->numberOfObstacles; i++)
	{
		if (this->IsColliding(player->GetBody(), this->obstacles[i]))
		{
			return true;
		}
	}
	return false;
}

SDL_Rect World::CreateRect(int x, int y, int width, int height)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	return rect;
}

bool World::IsColliding(SDL_Rect body1, SDL_Rect body2)
{
	if (body1.x < body2.x + body2.w && body1.x + body1.w > body2.x &&
		body1.y < body2.y + body2.h && body1.y + body1.h > body2.y)
	{
		return true;
	}
	return false;
}
