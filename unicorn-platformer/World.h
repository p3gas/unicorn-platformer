#pragma once
#include <SDL.h>
#include "Player.h"
class World
{
private:
	int levelWidth;
	int levelHeight;
	int numberOfPlatforms;
	int numberOfObstacles;
	SDL_Rect* platforms = NULL;
	SDL_Rect* obstacles = NULL;
public:
	int GetLevelWidth();
	int GetLevelHeight();
	void Build();
	void Draw(SDL_Renderer *renderer, const SDL_Rect* camera);
	void Destroy();
	void AdjustPlayerPosition(Player* player);
	bool IsCollidingWithObstacle(Player* player);
private:
	SDL_Rect CreateRect(int x, int y, int width, int height);
	bool IsColliding(SDL_Rect body1, SDL_Rect body2);
};
