#pragma once
#include "SDL.h"

class Render {
protected:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Renderer * renderer = NULL;
	SDL_Window* window = NULL;
public:
	Render()
	{
	}
	void createRenderer(SDL_Window* &window)
	{
		renderer = SDL_CreateRenderer(window, -1, 0);
	}
	void createWindow(const char *title)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	}
	SDL_Window *getWindow() { return this->window; }
	SDL_Renderer *getRenderer() { return this->renderer; }
	void render(SDL_Texture* sprite, SDL_Rect src, SDL_Rect dest, SDL_Texture* bg, SDL_Texture* barImg, SDL_Rect bar, bool right)
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopy(renderer, barImg, NULL, &bar);
		if (right)
			SDL_RenderCopy(renderer, sprite, &src, &dest);
		else
		{
			SDL_RenderCopyEx(renderer, sprite, &src,
				&dest, NULL, NULL, SDL_FLIP_HORIZONTAL);
		}
		SDL_RenderPresent(renderer);
	}
};


