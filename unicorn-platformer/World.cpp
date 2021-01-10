#include "World.h"
#include <stdio.h>

void World::Build(SDL_Texture* starTexture, SDL_Texture* stalactiteTexture)
{
	this->starTexture = starTexture;
	this->stalactiteTexture = stalactiteTexture;

	this->numberOfPlatforms = 13;
	this->levelWidth = 7680;
	this->levelHeight = 2160;
	this->startPosition = SDL_Point{ 0, 1160 };
	this->platforms = new SDL_Rect[this->numberOfPlatforms];
	this->platforms[0] = SDL_Rect{ 0, 1160, 640, 278 };
	this->platforms[1] = SDL_Rect{ 640, 1000, 1200, 278 };
	this->platforms[2] = SDL_Rect{ 940, 510, 640, 160 };
	this->platforms[3] = SDL_Rect{ 2100, 1160, 640, 278 };
	this->platforms[4] = SDL_Rect{ 3100, 1410, 1000, 278 };
	this->platforms[5] = SDL_Rect{ 3200, 860, 250, 200 };
	this->platforms[6] = SDL_Rect{ 3900, 1060, 1000, 170 };
	this->platforms[7] = SDL_Rect{ 4800, 810, 700, 278 };
	this->platforms[8] = SDL_Rect{ 5600, 1010, 1400, 278 };
	this->platforms[9] = SDL_Rect{ 7360, 1160, 320, 278 };
	this->platforms[10] = SDL_Rect{ 7680, 1160, 640, 278 };
	this->platforms[11] = SDL_Rect{ 8320, 1000, 700, 278 };
	this->platforms[12] = SDL_Rect{ -160, 1160, 160, 278 };

	this->numberOfStars = 4;
	this->stars = new Obstacle[this->numberOfStars];
	this->stars[0] = Obstacle(2320, 960, 200, 200, starTexture);
	this->stars[1] = Obstacle(3250, 710, 150, 150, starTexture);
	this->stars[2] = Obstacle(6000, 860, 150, 150, starTexture);
	this->stars[3] = Obstacle(6450, 860, 150, 150, starTexture);

	this->numberOfStalactites = 6;
	this->stalactites = new Obstacle[this->numberOfStalactites];
	this->stalactites[0] = Obstacle(1160, 670, 60, 150, stalactiteTexture);
	this->stalactites[1] = Obstacle(1230, 670, 60, 150, stalactiteTexture);
	this->stalactites[2] = Obstacle(1300, 670, 60, 150, stalactiteTexture);
	this->stalactites[3] = Obstacle(3250, 1060, 60, 150, stalactiteTexture);
	this->stalactites[4] = Obstacle(3320, 1060, 60, 150, stalactiteTexture);
	this->stalactites[5] = Obstacle(3990, 1230, 80, 190, stalactiteTexture);
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
