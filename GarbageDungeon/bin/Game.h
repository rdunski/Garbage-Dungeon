#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include "Sprite.h"
#include "Physics.h"
#include "Render.h"
//#include "SDL.h"
using namespace std;

class Game {
protected:
	Render renderer;
	bool done = false;
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Texture* bg = NULL;
	SDL_Texture* barImg = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect bar;
	SDL_Event e;

public:
	float getScreenHeight() { return this->SCREEN_HEIGHT; }
	float getScreenWidth() { return this->SCREEN_WIDTH; }
	SDL_Texture *getBackground() { return this->bg; }
	SDL_Surface *getSurface() { return this->surface; }

	void init() { SDL_Init(SDL_INIT_VIDEO); }
	void setRenderer(SDL_Window* window) { renderer.createRenderer(window); }

	void setBar()
	{
		bar.x = 10;
		bar.y = SCREEN_HEIGHT - 30;
		bar.w = 150;
		bar.h = 20;
		surface = SDL_LoadBMP("health_bar.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
		barImg = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	}

	void setBG()
	{
		surface = SDL_LoadBMP("Forest0.bmp");
		bg = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	}

	void checkWindowPos(Sprite &sprite) //"scene switching" (basically reverts position based on screen edge collision)
	{
		SDL_Rect tempdest = sprite.getdest();
		if (tempdest.x >= (getScreenWidth()))
		{
			tempdest.x = -20;
			sprite.setDestX(tempdest.x);
		}
		if (tempdest.x <= 0 - tempdest.w)
		{
			tempdest.x = getScreenWidth() - tempdest.w + 20;
			sprite.setDestX(tempdest.x);
		}
	}

	void eventHandler(Sprite &sprite)
	{
		sprite.setLast();
		if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
			sprite.move(barImg, bar, bg, sprite, SDL_SCANCODE_RIGHT);
		else if (currentKeyStates[SDL_SCANCODE_ESCAPE])
			done = true;
		else if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
			sprite.move(barImg, bar, bg, sprite, SDL_SCANCODE_LEFT);
		else if (currentKeyStates[SDL_SCANCODE_SPACE] || currentKeyStates[SDL_SCANCODE_UP])
			sprite.jump(barImg, bar, renderer.getRenderer(), bg, sprite);
	}

	void run()
	{
		renderer.createWindow("Garbage Dungeon", SCREEN_WIDTH, SCREEN_HEIGHT);
		setRenderer(renderer.getWindow());
		setBG();
		setBar();
		Sprite carl = carl.createSprite(renderer.getRenderer(), "duderight.bmp", 75, 80, 0, 275);
		while (!done)								// game loop
		{
			while (SDL_PollEvent(&e) != 0)			// exit check loop, also checking single-key presses
			{
				carl.setDT();
				if (e.type == SDL_QUIT)
					done = true;
				else if (e.type == SDL_KEYDOWN)
				{
					eventHandler(carl);
				}
			}
			checkWindowPos(carl); // check sprite pos and simulate switching "scenes"
			eventHandler(carl);   // checking for continuous key presses
			SDL_Delay(1000 / 24);
			SDL_RenderClear(renderer.getRenderer());
			renderer.renderBg(bg);
			renderer.renderHudObject(barImg, bar);
			renderer.renderSprite(carl.getSpriteTexture(), carl.isfacingright(), carl.getsrc(), carl.getdest());
			SDL_RenderPresent(renderer.getRenderer());
		}
		// clean up after ourselves
		endGame();
	}

	void endGame()
	{
		SDL_DestroyTexture(bg);
		SDL_DestroyTexture(barImg);
		SDL_FreeSurface(surface);
		SDL_DestroyRenderer(renderer.getRenderer());
		SDL_DestroyWindow(renderer.getWindow());
		
		SDL_Delay(100);
		SDL_Quit();
	}
};