#include "Player.h"

Player::Player()
{
	body = SDL_Rect();
	body.x = 0;
	body.y = 0;
	body.w = 200;
	body.h = 100;
	speed = 0.5f;
}

Player::Player(int x, int y, int width, int height, float speed)
{
	body = SDL_Rect();
	body.x = x;
	body.y = y;
	body.w = width;
	body.h = height;
	this->speed = speed;
}

void Player::Move(int x, int y)
{
	this->body.x += x;
	this->body.y += y;
}

void Player::SetPosition(int x, int y)
{
	this->body.x = x;
	this->body.y = y;
}

void Player::Draw(SDL_Renderer* renderer, const SDL_Rect* camera)
{
	SDL_Rect objectToBeDrew;
	objectToBeDrew.w = this->body.w;
	objectToBeDrew.h = this->body.h;
	objectToBeDrew.x = camera->x;
	objectToBeDrew.y = camera->y;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderDrawRect(renderer, &this->body);
	SDL_RenderDrawRect(renderer, &objectToBeDrew);
}

void Player::Update(int deltaTime)
{
	const Uint8* keyMap = SDL_GetKeyboardState(NULL);
	float x = 0.0f, y = 0.0f;

	if (keyMap[SDL_SCANCODE_UP])
	{
		y -= this->speed * deltaTime;
	}
	else if (keyMap[SDL_SCANCODE_DOWN])
	{
		y += this->speed * deltaTime;
	}
	if (keyMap[SDL_SCANCODE_RIGHT])
	{
		x += this->speed * deltaTime;
	}
	else if (keyMap[SDL_SCANCODE_LEFT])
	{
		x -= this->speed * deltaTime;
	}

	if (x != 0 && y != 0) {
		x /= 1.44f;
		y /= 1.44f;
	}

	this->Move(int(x), int(y));
}

int Player::GetOriginX()
{
	return (this->body.x + this->body.w) / 2;
}

int Player::GetOriginY()
{
	return (this->body.y + this->body.h) / 2;
}
