#include "Player.h"

Player::Player()
{
	body.w = 200;
	body.h = 164;
	speed = 0.5f;
	aAccelration = 0.000005f;
	jumpForce = 1.1f;
	gravity = 0.6f;
	isJumping = false;
	availableJumps = 2;
	jumpRange = 400;
	jumpKeyIsHold = false;
	dashRange = 400;
	dashSpeed = 4.0f;
	dashCooldownLeft = 0;
	dashCooldown = 1000;
	dashTraveled = 0;
	lives = 3;
}

void Player::Move(int x, int y)
{
	this->oldPosition.x = this->body.x;
	this->oldPosition.y = this->body.y;
	this->body.x += x;
	this->body.y += y;
}

void Player::SetPosition(int x, int y)
{
	this->oldPosition.x = this->body.x;
	this->oldPosition.y = this->body.y;
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

	if (this->controlMode == SIMPLE_MODE)
	{
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
	}
	else if (this->controlMode == ADVANCED_MODE)
	{
		if ((keyMap[SDL_SCANCODE_X] && dashCooldownLeft == 0))
		{
			this->isDashing = true;
			this->dashCooldownLeft = this->dashCooldown;
			if (this->availableJumps == 0)
			{
				this->availableJumps = 1;
			}
		}
		else if (!isDashing)
		{
			y = this->gravity * deltaTime;
			x = this->aSpeed * deltaTime;
			if (keyMap[SDL_SCANCODE_Z])
			{
				if (this->isJumping)
				{
					int toTravel = this->jumpTraveled - y + this->jumpForce * deltaTime;
					if (toTravel > this->jumpRange)
					{
						y = this->jumpTraveled - this->jumpRange;
						this->isJumping = false;
						this->jumpTraveled = 0;
					}
					y -= this->jumpForce * deltaTime;
					this->jumpTraveled += -int(y);
				}
				else
				{
					if (this->availableJumps > 0 && !this->jumpKeyIsHold)
					{
						this->availableJumps--;
						this->isJumping = true;
						y -= this->jumpForce * deltaTime;
						this->jumpTraveled += -int(y);
					}
				}
				this->jumpKeyIsHold = true;
			}
			else
			{
				this->jumpKeyIsHold = false;
				this->isJumping = false;
				this->jumpTraveled = 0;
			}
			this->Accelrate(deltaTime);
		}
	}

	if (this->isDashing)
	{
		x = this->dashSpeed * deltaTime;
		int toDash = this->dashTraveled + x;
		if (toDash > this->dashRange)
		{
			x = this->dashRange - this->dashTraveled;
			this->isDashing = false;
			this->dashTraveled = 0;
		}
		this->dashTraveled += x;
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
	this->dashCooldownLeft -= deltaTime;
	if (this->dashCooldownLeft < 0)
	{
		this->dashCooldownLeft = 0;
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

SDL_Point Player::GetOldPosition()
{
	return this->oldPosition;
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

void Player::ChangeControlMode()
{
	if (this->controlMode == SIMPLE_MODE)
	{
		this->controlMode = ADVANCED_MODE;
	}
	else
	{
		this->controlMode = SIMPLE_MODE;
		this->Reset();
	}
}

void Player::Reset()
{
	this->aSpeed = 0.45f;
	this->Ground();
}

void Player::Ground()
{
	this->isGrounded;
	this->jumpTraveled = 0;
	this->availableJumps = 2;
}

bool Player::IsDashing()
{
	return this->isDashing;
}

int Player::GetNumberOfLives()
{
	return this->lives;
}

bool Player::Dies()
{
	if (--this->lives <= 0)
	{
		return true;
	}
	return false;
}

void Player::ResetLives()
{
	this->lives = 3;
}

void Player::CorrectPosition(int x, int y)
{
	this->body.x = x;
	this->body.y = y;
}

void Player::Accelrate(int deltaTime)
{
	this->aSpeed += this->aAccelration * deltaTime;
}