#include "World.h"
#include <stdio.h>

void World::Build()
{
	this->numberOfPlatforms = 4;
	this->levelWidth = 5040;
	this->startPosition = SDL_Point{ 0, 1500 };
	this->platforms = new SDL_Rect[this->numberOfPlatforms];
	this->platforms[0] = SDL_Rect{ 0, 1500, 5040, 80 };
	this->platforms[1] = SDL_Rect{ 5040, 1500, 1280, 80 };
	this->platforms[2] = SDL_Rect{ 1700, 1200, 1280, 80 };
	this->platforms[3] = SDL_Rect{ -256, 1500, 256, 80 };

	this->numberOfObstacles = 6;
	this->stars = new SDL_Rect[this->numberOfObstacles];
	this->stars[0] = SDL_Rect{ 800, 1400, 100, 100 };
	this->stars[1] = SDL_Rect{ 1900, 1000, 100, 100 };
	this->stars[2] = SDL_Rect{ 3100, 1350, 100, 100 };
	this->stars[3] = SDL_Rect{ 4000, 900, 100, 100 };
	this->stars[4] = SDL_Rect{ 4700, 1200, 100, 100 };
	this->stars[5] = SDL_Rect{ 5840, 1400, 100, 100 };
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
		objectToBeDrew.w = this->stars[i].w;
		objectToBeDrew.h = this->stars[i].h;
		objectToBeDrew.x = this->stars[i].x - camera->x;
		objectToBeDrew.y = this->stars[i].y - camera->y;
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
	delete[]this->stars;
}

int World::AdjustPlayerPosition(Player* player)
{
	bool isPlayerGrounded = false;
	for (int i = 0; i < this->numberOfPlatforms; i++)
	{
		int collision = this->CheckIntersection(*player, this->platforms[i]);
		if (collision == FROM_UP)
		{
			player->CorrectPosition(player->GetBody().x, this->platforms[i].y - player->GetBody().h);
		}
		else if (collision != NONE)
		{
			return PLAYER_DEATH;
		}
		if (this->IsGrounded(*player, this->platforms[i]))
		{
			isPlayerGrounded = true;
		}
	}
	if (player->GetBody().x > this->levelWidth)
	{
		player->SetPosition(player->GetBody().x - this->levelWidth, player->GetBody().y);

	}
	
	if (isPlayerGrounded)
	{
		player->Ground();
	}
	else
	{
		player->isGrounded = false;
	}

	return NOTHING;
}

bool World::IsCollidingWithStar(Player* player)
{
	for (int i = 0; i < this->numberOfObstacles; i++)
	{
		int intersectionType = this->CheckIntersection(*player, this->stars[i]);
		if (intersectionType != NONE && intersectionType != INNER)
		{
			return true;
		}
	}
	return false;
}

SDL_Point World::GetStartPosition()
{
	return this->startPosition;
}

int World::CheckIntersection(Player player, SDL_Rect obstacle)
{
	if (player.GetBody().x < obstacle.x + obstacle.w && player.GetBody().x + player.GetBody().w > obstacle.x &&
		player.GetBody().y < obstacle.y + obstacle.h && player.GetBody().y + player.GetBody().h > obstacle.y)
	{
		if (player.GetOldPosition().y + player.GetBody().h <= obstacle.y)
		{
			return FROM_UP;
		}
		if (player.GetOldPosition().y >= obstacle.y + obstacle.h)
		{
			return FROM_DOWN;
		}
		if (player.GetOldPosition().x + player.GetBody().w <= obstacle.x)
		{
			return FROM_LEFT;
		}
		if (player.GetOldPosition().x >= obstacle.x + obstacle.w)
		{
			return FROM_RIGHT;
		}
		printf("LEFT DATA:\nw = %d, obs.x = %d\n", player.GetBody().w, obstacle.x);
		printf("INNER:\nx = %d, o_x = %d\ny = %d, o_y = %d\n\n", player.GetBody().x, player.GetOldPosition().x, player.GetBody().y, player.GetOldPosition().y);
		return INNER;
	}
	return NONE;
}

bool World::IsGrounded(Player player, SDL_Rect platform)
{
	if (((player.GetBody().x >= platform.x && player.GetBody().x <= platform.x + platform.w)
		|| (player.GetBody().x + player.GetBody().w >= platform.x && player.GetBody().x + player.GetBody().w <= platform.x + platform.w))
		&& player.GetBody().y + player.GetBody().h == platform.y)
	{
		return true;
	}
	return false;
}
