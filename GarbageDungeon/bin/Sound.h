#pragma once
#include "SDL.h"
#include "SDL_mixer.h"

class Sound {
protected:
	Mix_Music* hurt = NULL;
	Mix_Music* death = NULL;

public:
	
	void setSoundFiles()
	{
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
		hurt = Mix_LoadMUS("roblox-death-sound-effect.mp3");
		death = Mix_LoadMUS("WilhelmScream.mp3");
	}

	void playHurt()
	{
		Mix_PlayMusic(hurt, 1);
	}

	void playDeath()
	{
		Mix_PlayMusic(death, 1);
	}
};