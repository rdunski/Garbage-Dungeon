#include "SDL.h"
#include "Sound.h"
#include "Thread.h"

namespace thing {

	void destroyThread(Thread* thread)
	{
		SDL_WaitThread(thread->getThread(), NULL);
		SDL_DetachThread(thread->getThread());
	}

	int playStep(void *data)
	{
		Sound* tempData = reinterpret_cast<Sound*>(data);
		Mix_PlayMusic(tempData->getStep(), 1);
		return 0;
	}

	void startThread(Thread* thread, Sound* sound)
	{
		thread->setSoundThread(SDL_CreateThread(playStep, NULL, sound));
	}
}