#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "Sprite.h"
#include "SDL.h"
using namespace std;

class Game {
protected:
	bool done = false;
	//string bgfile = "Forest0.bmp";
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Window* window = NULL;
	SDL_Texture* bg = NULL;
	SDL_Surface* surface = NULL;
	SDL_Renderer* renderer = NULL;
	//Sprite *s;      // this isn't used at all, that I've seen.
	SDL_Event e;

public:
	void init() { SDL_Init(SDL_INIT_VIDEO); }

	void createWindow(SDL_Window* &window, string winName)
	{
		window = SDL_CreateWindow(winName.c_str(), SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	}

	void setBG(SDL_Window* window)
	{
		surface = SDL_LoadBMP("Forest0.bmp");
		renderer = SDL_CreateRenderer(window, -1, 0);
		bg = SDL_CreateTextureFromSurface(renderer, surface);
	}

	void run()
	{
		createWindow(this->window, "Garbage Dungeon");
		setBG(this->window);
		Sprite carl = carl.createSprite(renderer, "duderight.bmp", 75, 80, 0, 275);
		while (!done)								// game loop
		{
			while (SDL_PollEvent(&e) != 0)			// exit check loop
			{
				carl.setDT();
				if (e.type == SDL_QUIT)
				{
					done = true;
				}
			}

			SDL_Delay(1000 / 24);

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, carl.getSpriteTexture(), &carl.getsrc(), &carl.getdest());
			SDL_RenderPresent(renderer);

			eventHandler(carl);
		}
		// clean up after ourselves
		endGame();
	}

	SDL_Window *getWindow() { return this->window; }
	SDL_Texture *getBackground() { return this->bg; }
	SDL_Surface *getSurface() { return this->surface; }
	SDL_Renderer *getRenderer() { return this->renderer; }

	void eventHandler(Sprite &sprite) {
		// switchDirection causing huge memory leak
		// SD probably reloading surfaces every time
		
		// a solution would be to load both sprite sheets only once when the program 
		// is started and simply refer to whichever one is needed when it is needed.

		if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
		{
			//if (!sprite.isfacingright()) sprite.switchDirection(sprite, renderer);
			sprite.move(renderer, bg, sprite, SDL_SCANCODE_RIGHT);
		}
		else if (currentKeyStates[SDL_SCANCODE_ESCAPE])
			done = true;
		else if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
		{
			//if (sprite.isfacingright()) sprite.switchDirection(sprite, renderer);
			sprite.move(renderer, bg, sprite, SDL_SCANCODE_LEFT);
		}
		else if (currentKeyStates[SDL_SCANCODE_SPACE] || currentKeyStates[SDL_SCANCODE_UP])
		{
			sprite.jump(renderer, bg, sprite);
			// jump resets dest after each command
			sprite.move(renderer, bg, sprite, SDL_SCANCODE_SPACE);
		}
		else currentKeyStates[NULL];
	}

	void endGame()
	{
		SDL_DestroyTexture(bg);
		SDL_FreeSurface(surface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		
		SDL_Delay(100);
		SDL_Quit();
	}
};
