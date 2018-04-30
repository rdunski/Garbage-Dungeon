#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_thread.h"

class Sound {
protected:
	Mix_Music* hurt = NULL;
	Mix_Music* death = NULL;
	Mix_Music* step = NULL;
	Mix_Music* thud = NULL;

public:

	void setSoundFiles()
	{
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
		hurt = Mix_LoadMUS("roblox-death-sound-effect.mp3");
		death = Mix_LoadMUS("WilhelmScream.mp3");
		step = Mix_LoadMUS("soft_grass_footstep.mp3");
		thud = Mix_LoadMUS("dirt_jump_land.mp3");
	}

	void playHurt()
	{ Mix_PlayMusic(hurt, 1); }

	void playDeath()
	{ Mix_PlayMusic(death, 1); }

	void playThud()
	{ Mix_PlayMusic(thud, 1); }

	void killMusic()
	{
		Mix_FreeMusic(hurt);
		Mix_FreeMusic(death);
		Mix_FreeMusic(step);
		Mix_FreeMusic(thud);
	}
};