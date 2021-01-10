#pragma once
#include <SDL.h>
#include "Player.h"

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
	int numberOfObstacles;
	SDL_Point startPosition;
	SDL_Rect* platforms = NULL;
	SDL_Rect* obstacles = NULL;
public:
	int GetLevelWidth();
	int GetLevelHeight();
	void Build();
	void Draw(SDL_Renderer *renderer, const SDL_Rect* camera);
	void Destroy();
	int AdjustPlayerPosition(Player* player);
	bool IsCollidingWithObstacle(Player* player);
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
