#pragma once
#include <SDL.h>
#include <stdio.h>

enum ControlMode {
	SIMPLE_MODE = 1,
	ADVANCED_MODE
};

class Player
{
public:
	bool isGrounded = false;
private:
	SDL_Rect body;
	SDL_Point oldPosition;
	SDL_Texture* texture = NULL;
	float speed;
	int controlMode = SIMPLE_MODE;
	float aSpeed;
	float aMaxSpeed;
	float aAccelration;
	// JUMPING
	float gravity;
	float jumpForce;
	bool isJumping;
	int availableJumps;
	int jumpRange;
	int jumpTraveled;
	bool jumpKeyIsHold;
	// END OF JUMPING
	// DASH
	bool isDashing;
	int dashRange;
	int dashTraveled;
	float dashSpeed;
	int dashCooldownLeft;
	int dashCooldown;
public:
	Player();
	void Move(int x, int y);
	void SetPosition(int x, int y);
	void Draw(SDL_Renderer* renderer, const SDL_Rect* camera);
	void Update(int deltaTime);
	int GetOriginX();
	int GetOriginY();
	SDL_Rect GetBody();
	SDL_Point GetOldPosition();
	void SetTexture(SDL_Texture* texture);
	void Destroy();
	void ChangeControlMode();
	void Reset();
	void Ground();
private:
	void Accelrate(int deltaTime);
};
