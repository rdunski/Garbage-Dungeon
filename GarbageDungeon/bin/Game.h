#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include "Sprite.h"
#include "Physics.h"
#include "SDL.h"
//#include <sstream>
using namespace std;

class Game {
protected:
	bool done = false;
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Window* window = NULL;
	SDL_Texture* bg = NULL;
	SDL_Texture* barImg = NULL;
	SDL_Surface* surface = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Rect bar;
	SDL_Event e;

public:
	float getScreenHeight() { return this->SCREEN_HEIGHT; }
	float getScreenWidth() { return this->SCREEN_WIDTH; }

	SDL_Window *getWindow() { return this->window; }
	SDL_Texture *getBackground() { return this->bg; }
	SDL_Surface *getSurface() { return this->surface; }
	SDL_Renderer *getRenderer() { return this->renderer; }

	void init() { SDL_Init(SDL_INIT_VIDEO); }

	void createWindow(SDL_Window* &window, string winName)
	{
		window = SDL_CreateWindow(winName.c_str(), SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	}

	void setBar()
	{
		bar.x = 10;
		bar.y = SCREEN_HEIGHT-30;
		bar.w = 150;
		bar.h = 20;
		surface = SDL_LoadBMP("health_bar.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
		barImg = SDL_CreateTextureFromSurface(renderer, surface);
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
		setBar();
		Sprite carl = carl.createSprite(renderer, "duderight.bmp", 75, 80, 0, 275);
		while (!done)								// game loop
		{
			while (SDL_PollEvent(&e) != 0)			// exit check loop, also checking single-key presses
			{
				if (e.type == SDL_QUIT)
					done = true;
				else if (e.type == SDL_KEYDOWN)
				{
					eventHandler(carl);
				}
			}
			checkWindowPos(carl); // check sprite pos and simulate switching "scenes"
			eventHandler(carl); // checking for continuous key presses
			SDL_Delay(1000 / 24);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, barImg, NULL, &bar);
			if (carl.isfacingright())
				SDL_RenderCopy(renderer, carl.getSpriteTexture(), &carl.getsrc(), &carl.getdest());
			else
			{
				SDL_RenderCopyEx(renderer, carl.getSpriteTexture(), &carl.getsrc(),
					&carl.getdest(), NULL, NULL, SDL_FLIP_HORIZONTAL);
			}
			SDL_RenderPresent(renderer);
		}
		// clean up after ourselves
		endGame();
	}

	void checkWindowPos(Sprite &sprite) //"scene switching" (basically reverts position based on screen edge collision)
	{
		SDL_Rect tempdest = sprite.getdest();
		if (tempdest.x >= (getScreenWidth()))
		{
			tempdest.x = -20;
			sprite.setDestX(tempdest.x);
		}
		if (tempdest.x <= 0-tempdest.w)
		{
			tempdest.x = getScreenWidth() - tempdest.w + 20;
			sprite.setDestX(tempdest.x);
		}
	}

	void eventHandler(Sprite &sprite) 
	{
		if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
			sprite.move(renderer, bg, sprite, SDL_SCANCODE_RIGHT);
		else if (currentKeyStates[SDL_SCANCODE_ESCAPE])
			done = true;
		else if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
			sprite.move(renderer, bg, sprite, SDL_SCANCODE_LEFT);
		else if (currentKeyStates[SDL_SCANCODE_SPACE] || currentKeyStates[SDL_SCANCODE_UP])
			sprite.jump(renderer, bg, sprite);
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
