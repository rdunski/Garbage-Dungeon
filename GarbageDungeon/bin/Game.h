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
	bool done, dead, moving, jumping = false;
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Texture* bg = NULL;
	SDL_Texture* barImg = NULL;
	SDL_Texture* revive = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect bar;
	SDL_Rect barSrc;
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
		barSrc.x = 0;
		barSrc.y = 0;
		barSrc.w = 75;
		barSrc.h = 10;
		surface = SDL_LoadBMP("health_bar_sheet.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
		barImg = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	}

	void setBG()
	{
		surface = SDL_LoadBMP("Forest0.bmp");
		bg = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	}

	void setRevive()
	{
		surface = SDL_LoadBMP("revive.bmp");
		revive = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
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
		if (sprite.getHealth() <= 0)
			dead = true;
		int tempHealth = sprite.getHealth();
		sprite.setLast();
		if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
		{
			sprite.move(barImg, bar, barSrc, bg, sprite, SDL_SCANCODE_RIGHT);
			moving = true;
		}
		if (currentKeyStates[SDL_SCANCODE_ESCAPE])
			done = true;
		if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
		{
			sprite.move(barImg, bar, barSrc, bg, sprite, SDL_SCANCODE_LEFT);
			moving = true;
		}
		if (sprite.getJumpSrc().x >= 968)
			sprite.drop(sprite);
		if ((currentKeyStates[SDL_SCANCODE_SPACE] || currentKeyStates[SDL_SCANCODE_UP]) && sprite.getJumpSrc().x <968)
		{
			sprite.beginJump(sprite);
			jumping = true;
		}
		else if (sprite.getdest().y < 275)
			jumping = true;
		if (currentKeyStates[SDL_SCANCODE_H] && !dead) //HARM
		{
			tempHealth = tempHealth - 10;
			sprite.setHealth(tempHealth);
		}
		if (currentKeyStates[SDL_SCANCODE_G]) //KILL
			sprite.setHealth(0);
		if (currentKeyStates[SDL_SCANCODE_R]) //REVIVE
			sprite.setHealth(100);
	}

	void setup()
	{
		renderer.createWindow("Garbage Dungeon", SCREEN_WIDTH, SCREEN_HEIGHT);
		setRenderer(renderer.getWindow());
		setBG();
		setBar();
		setRevive();
	}

	void run()
	{
		setup();
		Sprite carl = carl.createSprite(renderer.getRenderer(), 75, 80, 0, 275);
		carl.setHealth(100);
		while (!done)								// game loop
		{
			while (dead) //stay dead until revived or quit
			{
				SDL_RenderClear(renderer.getRenderer());
				SDL_Delay(1000 / 24);
				renderer.renderBg(revive);
				renderer.renderHudObject(barImg, barSrc, bar);
				SDL_RenderPresent(renderer.getRenderer());
				while (SDL_PollEvent(&e) != 0)
				{
					carl.setDT();
					if (e.type == SDL_QUIT)
						endGame();
					else if (currentKeyStates[SDL_SCANCODE_R])
					{
						dead = false;
						carl.setDestX(0);
						carl.setSrcX(0);
						carl.setSrcY(0);
						carl.setHealth(100);
					}
					if (currentKeyStates[SDL_SCANCODE_ESCAPE])
						endGame();
				}
			}
			SDL_Delay(1000 / 24);
			while (SDL_PollEvent(&e) != 0)			// exit check loop, also checking single-key presses
			{
				carl.setDT();
				if (e.type == SDL_QUIT)
					done = true;
				else if (e.type == SDL_KEYDOWN)
					eventHandler(carl);
			}
			checkWindowPos(carl); // check sprite pos and simulate switching "scenes"
			eventHandler(carl);   // checking for continuous key presses
			SDL_RenderClear(renderer.getRenderer());
			renderer.renderBg(bg);
			checkHealth(carl);
			renderer.renderHudObject(barImg, barSrc, bar);
			if (moving && !jumping)
			{
				renderer.renderSprite(carl.getSpriteMotionTexture(), carl.isfacingright(), carl.getsrc(), carl.getdest());
				carl.setJumpSrcX(0);
			}
			if (jumping || (jumping && moving))
			{
				renderer.renderSprite(carl.getSpriteJumpTexture(), carl.isfacingright(), carl.getJumpSrc(), carl.getdest());
				if (carl.getJumpSrc().x == 0)
					carl.setJumpSrcX(121);
			}
			if (!moving && !jumping)
			{
				carl.setJumpSrcX(0);
				carl.setStandDestX(carl.getdest().x);
				renderer.renderSprite(carl.getSpriteStandTexture(), carl.isfacingright(), carl.getStandSrc(), carl.getStandDest());
				if (carl.getStandSrc().x < 2052)
					carl.setStandSrcX(carl.getStandSrc().x + 92);
				if (carl.getStandSrc().x >= 2052)
					carl.setStandSrcX(0);
				renderer.renderSprite(carl.getSpriteStandTexture(), carl.isfacingright(), carl.getStandSrc(), carl.getStandDest());
			}
			moving = false;
			jumping = false;
			SDL_RenderPresent(renderer.getRenderer());
		}
		// clean up after ourselves
		endGame();
	}

	void checkHealth(Sprite sprite)
	{
		if(sprite.getHealth() <0)
			sprite.setHealth(100);
		int hp = sprite.getHealth();
		switch (hp)
		{
		case 100:
			barSrc.x = 0;
			barSrc.y = 0;
			break;
		case 90:
			barSrc.x = 0;
			barSrc.y = 11;
			break;
		case 80:
			barSrc.x = 0;
			barSrc.y = 22;
			break;
		case 70:
			barSrc.x = 0;
			barSrc.y = 33;
			break;
		case 60:
			barSrc.x = 0;
			barSrc.y = 44;
			break;
		case 50:
			barSrc.x = 80;
			barSrc.y = 0;
			break;
		case 40:
			barSrc.x = 80;
			barSrc.y = 11;
			break;
		case 30:
			barSrc.x = 80;
			barSrc.y = 22;
			break;
		case 20:
			barSrc.x = 80;
			barSrc.y = 33;
			break;
		case 10:
			barSrc.x = 80;
			barSrc.y = 44;
			break;
		case 0:
			dead = true;
			barSrc.x = 80;
			barSrc.y = 55;
			break;
		}
	}

	void endGame()
	{
		SDL_DestroyTexture(bg);
		SDL_DestroyTexture(barImg);
		SDL_DestroyTexture(revive);
		SDL_FreeSurface(surface);
		SDL_DestroyRenderer(renderer.getRenderer());
		SDL_DestroyWindow(renderer.getWindow());
		
		SDL_Delay(100);
		SDL_Quit();
	}
};