#pragma once
#include <stdio.h>
#include <iostream>
#include<string>
#include<sstream>
#include"Sprite.h"
#include "SDL.h"
using namespace std;

class Game {
protected:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Window* window = NULL;
	SDL_Texture* bg = NULL;
	SDL_Surface* surface = NULL;
	SDL_Renderer* renderer = NULL;
	Sprite *s;
	SDL_Event e;
public:
	void init() {SDL_Init(SDL_INIT_VIDEO);}

	void run(string winName, string bgfile)
	{
		createWindow(this->window, winName);
		setBG(this->window, bgfile);
	}

	SDL_Window *getWindow() { return this->window;}
	SDL_Texture *getBackground() { return this->bg; }
	SDL_Surface *getSurface() { return this->surface; }
	SDL_Renderer *getRenderer() { return this->renderer; }

	void createWindow(SDL_Window* &window, string winName) {
		window = SDL_CreateWindow(winName.c_str(), SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	}

	void setBG(SDL_Window* window, string &filename)
	{
		surface = SDL_LoadBMP(filename.c_str());
		renderer = SDL_CreateRenderer(window, -1, 0);
		bg = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(1000);
	}

	void eventHandler(SDL_Event e, SDL_Renderer* renderer, SDL_Texture* sprite, SDL_Rect &bgsrc, SDL_Rect &bgdest, bool &quit, bool &right) {
		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym)
			{
			case SDLK_RIGHT:
				if (!right) s->switchDirection(surface, sprite, renderer, right);
				s->runright(renderer, bg, sprite, bgsrc, bgdest);
				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_LEFT:
				if (right) s->switchDirection(surface, sprite, renderer, right);
				s->runleft(renderer, bg, sprite, bgsrc, bgdest, right);
				break;
			case SDLK_UP:
				s->jump(renderer, bg, sprite, bgsrc, bgdest, right);
				break;
			}
		}
	}

void endGame(SDL_Window* window)
{
	//Wait two seconds
	SDL_Delay(100);
	//Destroy window
	SDL_DestroyWindow(window);
	//Quit SDL subsystems
	SDL_Quit();
}
};