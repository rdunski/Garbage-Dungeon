#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_thread.h"

class Sound {
protected:
	Mix_Chunk* hurt = NULL;
	Mix_Chunk* death = NULL;
	Mix_Chunk* step = NULL;
	Mix_Chunk* thud = NULL;

public:

	void setSoundFiles()
	{
		Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 640);
		hurt = Mix_LoadWAV("roblox-death-sound-effect.wav");
		death = Mix_LoadWAV("WilhelmScream.wav");
		step = Mix_LoadWAV("soft_grass_footstep.wav");
		thud = Mix_LoadWAV("dirt_jump_land.wav");
	}

	void playHurt() { Mix_PlayChannel(-1, hurt,1); }
	void playDeath() { Mix_PlayChannel(-1, death, 1); }
	void playThud() { Mix_PlayChannel(-1, thud, 1); }
	void playStep() { Mix_PlayChannel(1, step, 1); }

	bool isPlaying(int channel) { return Mix_Playing(channel); }

	void stopChannel(int channel) { Mix_HaltChannel(channel); }

	void killMusic()
	{
		Mix_FreeChunk(hurt);
		Mix_FreeChunk(death);
		Mix_FreeChunk(step);
		Mix_FreeChunk(thud);
	}
};