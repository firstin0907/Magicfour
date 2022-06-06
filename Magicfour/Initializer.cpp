#include "Initializer.h"
#include "global.h"


Initializer::Initializer()
{
	// initialize SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw std::exception("Failed to initialize SDL2");
	}

	// initialize SDL_image
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		throw std::exception("Failed to initialize SDL_image");
	}

	// initialize SDL_mixer
	if (Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 4096) == -1)
	{
		throw std::exception("Failed to initialize SDL_mixer");
	}

	// initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		throw std::exception("Failed to initialize SDL_ttf");
	}
}


Initializer::~Initializer()
{
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
