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
	bool done = false;
	string bgfile = "Forest0.bmp";
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Window* window = NULL;
	SDL_Texture* bg = NULL;
	SDL_Surface* surface = NULL;
	SDL_Renderer* renderer = NULL;
	Sprite *s;
	SDL_Event e;
public:
	void init() { SDL_Init(SDL_INIT_VIDEO); }

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
	}

	void run() //This needs to go in a class for game instance
	{
		createWindow(this->window, "Garbage Dungeon");
		setBG(this->window, bgfile);
		//Sprite shield = shield.createSprite(renderer, "background.bmp");
		Sprite carl = carl.createSprite(renderer, "duderight.bmp");
		while (!done)
		{
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, carl.getSprite(), &carl.getsrc(), &carl.getdest());
			SDL_RenderPresent(renderer);
			if (SDL_PollEvent(&e)) eventHandler(e, carl);
		}
	}

	SDL_Window *getWindow() { return this->window; }
	SDL_Texture *getBackground() { return this->bg; }
	SDL_Surface *getSurface() { return this->surface; }
	SDL_Renderer *getRenderer() { return this->renderer; }

	void eventHandler(SDL_Event e, Sprite &sprite) {
		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym)
			{
			case SDLK_RIGHT:
				if (!right) sprite.switchDirection(sprite, sprite.getSprite(), renderer);
				sprite.runright(renderer, bg, sprite.getSprite(), sprite.getsrc(), sprite.getdest());
				break;
			case SDLK_ESCAPE:
				done = true;
				break;
			case SDLK_LEFT:
				if (right) sprite.switchDirection(sprite, sprite.getSprite(), renderer);
				sprite.runleft(renderer, bg, sprite.getSprite(), sprite.getsrc(), sprite.getsrc());
				break;
			case SDLK_UP:
				sprite.jump(renderer, bg, sprite.getSprite(), sprite.getsrc(), sprite.getsrc());
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

/*
	(LEARN GIT!!!)
	-->PHYSICS<--
	Scrolling
	Beef up event handler (pick up, contact, etc)
	Story (THAT'S ON ME)

	doug - cody - levi
	cameron - jacob
	jeremy - nick - warren(?)
	mitchell - marcus - matt
	DAN - sean - robert
	sayer - lucas
	tyler - daniel - nicholas
	-------------------------------------------------------------------------------

	Java -> "synchronized" void, one at a time (keep small
			Threading: "runnable" function 
				void run
				Thread class -> start (runnable)
	C++ -> SDL Thread Management: https://wiki.libsdl.org/SDL_CreateThread
			
			Threads:	Rendering loop (fps goal)
						Event Loop (event handler)
						Physics Loop 

	(void *) --> (int *) cnt;
 
	SDL_Thread *thread, *thread2;
	int threadretval, threadretval2;



*/