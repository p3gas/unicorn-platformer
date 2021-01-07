#pragma once
#include <SDL.h>

class Player
{
private:
	SDL_Rect body;
	SDL_Texture* texture = NULL;
	float speed;
public:
	Player();
	void Move(int x, int y);
	void SetPosition(int x, int y);
	void Draw(SDL_Renderer* renderer, const SDL_Rect* camera);
	void Update(int deltaTime);
	int GetOriginX();
	int GetOriginY();
	SDL_Rect GetBody();
	void SetTexture(SDL_Texture* texture);
	void Destroy();
};
