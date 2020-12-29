//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include "Configuration.h"
#include "GameManager.h"


int main(int argc, char* args[])
{
	Configuration configuration;
	GameManager gameManager(&configuration);

	{
		int errorCode = gameManager.Init();
		if (errorCode != 0) {
			return errorCode;
		}
	}
	{
		int errorCode = gameManager.LoadResources();
		if (errorCode != 0) {
			return errorCode;
		}
	}
	Uint32 lastTime = SDL_GetTicks();
	while (gameManager.IsRunning()) {
		int milisecondsElapsed = SDL_GetTicks() - lastTime;
		if (milisecondsElapsed < 1000 / float(configuration.fpsLimit))
		{
			SDL_Delay(1000 / float(configuration.fpsLimit) - milisecondsElapsed);
			milisecondsElapsed = SDL_GetTicks() - lastTime;
		}
		lastTime = SDL_GetTicks();
		gameManager.ProcessInput();
		gameManager.Update(milisecondsElapsed / 1000.0f);
		gameManager.Render();
	}
	gameManager.Quit();

	return 0;
}
