#include "Player.h"

Player::Player()
{
	body = SDL_Rect();
	body.w = 200;
	body.h = 164;
	speed = 0.5f;
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
	objectToBeDrew.x = this->body.x - camera->x;
	objectToBeDrew.y = this->body.y - camera->y;
	SDL_RenderCopy(renderer, this->texture, NULL, &objectToBeDrew);
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
	if (this->body.x < 0)
	{
		this->body.x = 0;
	}
	if (this->body.y < 0)
	{
		this->body.y = 0;
	}
}

int Player::GetOriginX()
{
	return this->body.x + this->body.w / 2;
}

int Player::GetOriginY()
{
	return this->body.y + this->body.h / 2;
}

SDL_Rect Player::GetBody()
{
	return this->body;
}

void Player::SetTexture(SDL_Texture* texture)
{
	this->texture = texture;
}

void Player::Destroy()
{
	SDL_DestroyTexture(this->texture);
	this->texture = NULL;
}
