#pragma once
#include <SDL.h>

class Player
{
private:
	SDL_Rect body;
	float speed;
public:
	Player();
	Player(int x, int y, int width, int height, float speed);
	void Move(int x, int y);
	void SetPosition(int x, int y);
	void Draw(SDL_Renderer* renderer, const SDL_Rect* camera);
	void Update(int deltaTime);
	int GetOriginX();
	int GetOriginY();
};
