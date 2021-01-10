#pragma once
class Configuration
{
public:
	const int screenWidth = 1280;
	const int screenHeight = 720;
	const int topBarHight = 100;
	const char* backgroundTexturePath = "images/background.png";
	const char* menuTexturePath = "images/menuBackground.png";
	const char* playerTexturePath = "images/unicorn.png";
	const char* starTexturePath = "images/star.png";
	const char* stalactiteTexturePath = "images/stalaktyt.png";
	const char* heartTexturePath = "images/heart.png";
	const char* confirmationTexturePath = "images/confirmation.png";
	const char* pathToFont = "fonts/ui_font.ttf";
	const int fpsLimit = 300;
};
