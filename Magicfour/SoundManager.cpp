#include "SoundManager.h"



SoundManager::SoundManager()
{
	Mix_AllocateChannels(16);
	
	std::string path = "Resource\\Sound\\music";
	for (int i = 0; i < 1; i++)
	{
		auto loaded_music = Mix_LoadMUS((path + std::to_string(i) + ".mp3").c_str());
		if (loaded_music == nullptr)
		{
			std::cout << "Warning :: Failed to load music  : " << Mix_GetError() << std::endl;
		}
		music.push_back(loaded_music);
	}

	path = "Resource\\Sound\\sfx";
	for (int i = 0; i < 5; i++)
	{
		auto loaded_sfx = Mix_LoadWAV((path + std::to_string(i) + ".wav").c_str());
		if (loaded_sfx == nullptr)
		{
			std::cout << "Warning :: Failed to load sfx : " << Mix_GetError() << std::endl;
		}
		sfx.push_back(loaded_sfx);
	}
}

void SoundManager::play_music(int music_id)
{
	Mix_HaltMusic();

	if (music_id < music.size() && music[music_id] != nullptr)
	{
		if (Mix_PlayMusic(music[music_id], -1) == -1)
		{
			std::cout << "Warning :: Fail to play music : " << Mix_GetError() << std::endl;
		}
	}
}

void SoundManager::stop_music()
{
	Mix_HaltMusic();
}


void SoundManager::play_sfx(int sfx_id)
{
	if (sfx_id < sfx.size() && sfx[sfx_id] != nullptr)
	{
		if (Mix_PlayChannel(-1, sfx[sfx_id], 0) == -1)
		{
			std::cout << "Warning :: Fail to play sfx : " << Mix_GetError() << std::endl;
		}
	}
}

SoundManager::~SoundManager()
{
	for (auto &i : music) Mix_FreeMusic(i);
	
	for (auto &i : sfx) Mix_FreeChunk(i);
}
