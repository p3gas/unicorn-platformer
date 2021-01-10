#include "World.h"
#include <stdio.h>

void World::Build(SDL_Texture* starTexture, SDL_Texture* stalactiteTexture)
{
	this->starTexture = starTexture;
	this->stalactiteTexture = stalactiteTexture;

	this->levelWidth = 5040;
	this->levelHeight = 1700;
	this->startPosition = SDL_Point{ 0, 1500 };

	this->numberOfPlatforms = 4;
	this->platforms = new SDL_Rect[this->numberOfPlatforms];
	this->platforms[0] = SDL_Rect{ 0, 1500, 5040, 80 };
	this->platforms[1] = SDL_Rect{ 5040, 1500, 1280, 80 };
	this->platforms[2] = SDL_Rect{ 1700, 1200, 1280, 80 };
	this->platforms[3] = SDL_Rect{ -256, 1500, 256, 80 };

	this->numberOfStars = 6;
	this->stars = new Obstacle[this->numberOfStars];
	this->stars[0] = Obstacle(800, 1400, 100, 100, starTexture);
	this->stars[1] = Obstacle(1900, 1000, 100, 100, starTexture);
	this->stars[2] = Obstacle(3100, 1350, 100, 100, starTexture);
	this->stars[3] = Obstacle(4000, 900, 100, 100, starTexture);
	this->stars[4] = Obstacle(4700, 1200, 100, 100, starTexture);
	this->stars[5] = Obstacle(5840, 1400, 100, 100, starTexture);

	this->numberOfStalactites = 1;
	this->stalactites = new Obstacle[this->numberOfStalactites];
	this->stalactites[0] = Obstacle(1700, 1000, 50, 150, stalactiteTexture);
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
	for (int i = 0; i < this->numberOfStars; i++)
	{
		this->stars[i].Draw(renderer, camera);
	}
	for (int i = 0; i < this->numberOfStalactites; i++)
	{
		this->stalactites[i].Draw(renderer, camera);
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
	delete[]this->stalactites;
	SDL_DestroyTexture(this->starTexture);
	this->starTexture = NULL;
	SDL_DestroyTexture(this->stalactiteTexture);
	this->stalactiteTexture = NULL;
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
	if (player->GetBody().y + player->GetBody().h > this->levelHeight)
	{
		return PLAYER_DEATH;
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
	for (int i = 0; i < this->numberOfStars; i++)
	{
		int intersectionType = this->CheckIntersection(*player, this->stars[i].GetBody());
		if (intersectionType != NONE && intersectionType != INNER)
		{
			return true;
		}
	}
	return false;
}

bool World::IsCollidingWithStalactite(Player* player)
{
	for (int i = 0; i < this->numberOfStalactites; i++)
	{
		int intersectionType = this->CheckIntersection(*player, this->stalactites[i].GetBody());
		if (intersectionType != NONE)
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
