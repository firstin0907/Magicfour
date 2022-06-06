#pragma once
#include "header.h"

class SoundManager
{
protected:
	std::vector<Mix_Music*> music;
	std::vector<Mix_Chunk*> sfx;

public:
	SoundManager();

	void play_music(int music_id);
	void stop_music();

	void play_sfx(int sfx_id);

	~SoundManager();
};

