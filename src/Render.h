#pragma once
#include "SDL.h"

class Render {
protected:
	SDL_Renderer* renderer = NULL;	// the renderer...
	SDL_Window* window = NULL;		// the window...

public:
	Render()
	{
	}

	// getters
	SDL_Window *getWindow() { return this->window; }
	SDL_Renderer *getRenderer() { return this->renderer; }

	void createRenderer(SDL_Window* &window) // create renderer with window...
	{
		renderer = SDL_CreateRenderer(window, -1, 0);
	}

	void createWindow(const char *title, int SCREEN_WIDTH, int SCREEN_HEIGHT)		// create window
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_WINDOW_SHOWN, SDL_WINDOW_RESIZABLE)); // supports resizing
	}

	void renderBg(SDL_Texture* bg) // render background
	{
		SDL_RenderCopy(renderer, bg, NULL, NULL);
	}

	void renderHudObject(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest)			// render other objects
	{
		SDL_RenderCopy(renderer, texture, &src, &dest);
	}

	void renderSprite(SDL_Texture* sprite, bool right, SDL_Rect src, SDL_Rect dest)	// render sprite...
	{
		if (right)
			SDL_RenderCopy(renderer, sprite, &src, &dest);
		else
		{
			SDL_RenderCopyEx(renderer, sprite, &src,
				&dest, NULL, NULL, SDL_FLIP_HORIZONTAL);
		}
	}
};