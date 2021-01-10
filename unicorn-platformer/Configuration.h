#pragma once
class Configuration
{
public:
	const int screenWidth = 1280;
	const int screenHeight = 720;
	const int topBarHight = 100;
	const char* backgroundTexturePath = "images/background.png";
	const char* playerTexturePath = "images/unicorn.png";
	const char* pathToFont = "fonts/ui_font.ttf";
	const int fpsLimit = 300;
};

