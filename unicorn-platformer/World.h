#pragma once
#include <SDL.h>
#include "Player.h"
#include "Obstacle.h"

enum WorldEvent {
	NOTHING,
	PLAYER_DEATH
};

class World
{
private:
	int levelWidth;
	int levelHeight;
	int numberOfPlatforms;
	int numberOfStars;
	int numberOfStalactites;
	SDL_Point startPosition;
	SDL_Rect* platforms = NULL;
	Obstacle* stars = NULL;
	Obstacle* stalactites = NULL;
	SDL_Texture* starTexture = NULL;
	SDL_Texture* stalactiteTexture = NULL;
public:
	int GetLevelWidth();
	int GetLevelHeight();
	void Build(SDL_Texture* starTexture, SDL_Texture* stalactiteTexture);
	void Draw(SDL_Renderer *renderer, const SDL_Rect* camera);
	void Destroy();
	int AdjustPlayerPosition(Player* player);
	bool IsCollidingWithStar(Player* player);
	bool IsCollidingWithStalactite(Player* player);
	SDL_Point GetStartPosition();
private:
	int CheckIntersection(Player player, SDL_Rect obstacle);
	bool IsGrounded(Player player, SDL_Rect platform);
};

enum IntersectionDirection
{
	NONE,
	FROM_LEFT,
	FROM_RIGHT,
	FROM_UP,
	FROM_DOWN,
	INNER
};
