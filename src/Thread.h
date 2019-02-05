#pragma once
#include "SDL.h"
#include "SDL_thread.h"
#include "Sound.h"

class Thread {
protected:
	SDL_Thread * soundThread = NULL;		//the sound thread to play various things on
public:
	SDL_Thread * getThread() { return this->soundThread; }

	void setSoundThread(SDL_Thread* thread) { this->soundThread = thread; }

	void stopThread()
	{
		thing::destroyThread(this);
	}
};

//void thing::startThread(Thread* thread, Sound* sound);
//void thing::destroyThread(Thread* thread);
//int thing::playStep(void *data);
