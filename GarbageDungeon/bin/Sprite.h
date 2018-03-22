#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "Game.h"
#include "SDL.h"
using namespace std;

class Sprite {
protected:
	SDL_Texture * text = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect src; // src is manipulating sprite sheet
	SDL_Rect dest; // dest is sprite position on screen
		// changeable includes x, y (instead of pX, pY)
	bool right = true;
	float vX, vY; // velocity
	float aX, aY; // acceleration
	float last, dt; // varibles for timing the physics

	//will be moving filename here, creating 
	//getter/setter for switchdirection function

public:
	Sprite() 
	{
		vX = 100.0;
		vY = 100.0;
		aX = 100.0;
		aY = 100.0;
		dt = 0.0;
		last = 0.0;
	}

	/*void updateMovement(float &pVar, float &vVar, float &aVar, float dt) 
	{
		last = SDL_GetTicks();
		vVar = vVar + (aVar * dt);
		pVar = pVar + (vVar * dt);
	}*/

	void setDT() { dt = ((float)SDL_GetTicks() - last) / (float)1000.0; }

	Sprite createSprite(SDL_Renderer* renderer, string filename,
		int srcx, int srcy, int destx, int desty)
	{
		Sprite newS;
		newS.src = { 0, 0, srcx, srcy };
		newS.dest.x = destx; newS.dest.y = desty; newS.dest.h = 75; newS.dest.w = 80;
		newS.surface = SDL_LoadBMP(filename.c_str());
		SDL_SetColorKey(newS.surface, SDL_TRUE, SDL_MapRGB(newS.surface->format, 0, 0, 0));
		newS.text = SDL_CreateTextureFromSurface(renderer, newS.surface);
		return newS;
	}

	SDL_Surface* getSurface() { return this->surface; }
	SDL_Texture* getSpriteTexture() { return this->text; }
	SDL_Rect getsrc() { return this->src; }
	SDL_Rect getdest() { return this->dest; }
	bool isfacingright() { return right; }

	void switchDirection(Sprite sprite, SDL_Renderer* renderer)
	{
		//If sprite is facing one direction and the other is called, 
		//should load a new bmp of character to use
		if (sprite.isfacingright() && SDLK_LEFT)
		{
			sprite.surface = SDL_LoadBMP("dudeleft.bmp");
			SDL_SetColorKey(sprite.surface, SDL_TRUE, SDL_MapRGB(sprite.surface->format, 0, 0, 0));
			sprite.text = SDL_CreateTextureFromSurface(renderer, sprite.surface);
			sprite.right = false;
		}
		else if (!sprite.isfacingright() && SDLK_RIGHT)
		{
			sprite.surface = SDL_LoadBMP("dudeleft.bmp");
			SDL_SetColorKey(sprite.surface, SDL_TRUE, SDL_MapRGB(sprite.surface->format, 0, 0, 0));
			sprite.text = SDL_CreateTextureFromSurface(renderer, sprite.surface);
			sprite.right = true;
		}
		sprite.text = SDL_CreateTextureFromSurface(renderer, sprite.surface);
	}

	void move(SDL_Renderer* renderer, SDL_Texture* bg, Sprite sprite, SDL_Scancode keystate)
	{
		/*SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);*/
		// The above might be redundant and causing things to reset.

		if (sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			//if sprite is facing right and moves right
			sprite.dest.x += 100;
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			//if sprite is facing left and moves left
		}
		else if (sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			//if sprite is facing right and moves left
			switchDirection(sprite, renderer);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			//if sprite is left and moves right
			switchDirection(sprite, renderer);
		}

		SDL_RenderCopy(renderer, bg, NULL, NULL);											// always put this before rendering something else?
		SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
		SDL_RenderPresent(renderer);
		
	}

	void jump(SDL_Renderer* renderer, SDL_Texture* bg, Sprite sprite)
	{
		//This whole function should be scrapped for one using correct physics. 
		//Will be more complicated than simple movement, so it should probably 
		//stay in it's own function
		bool complete = false;
		while (!complete)
		{
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			SDL_RenderPresent(renderer);

			if (sprite.dest.y < 175) complete = true;
			sprite.dest.x += 6;
			sprite.dest.y -= 8;
			SDL_Delay(1000 / 24); // set framerate to 24 FPS
			SDL_RenderClear(renderer);
			if (sprite.src.y < 255)
			{
				if (sprite.src.x < 450) sprite.src.x += 75;
				else {
					sprite.src.x = 0;
					sprite.src.y += 85;
				}
			}
			else
			{
				if (sprite.src.x < 375) sprite.src.x += 75;
				else {
					sprite.src.x = 0;
					sprite.src.y = 0;
				}
			}
		}
		while (complete)
		{
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			SDL_RenderPresent(renderer);
			if (sprite.dest.y >= 275) complete = false;
			sprite.dest.x += 5;
			sprite.dest.y += 10;
			SDL_Delay(1000 / 24);
			SDL_RenderClear(renderer);
			if (sprite.src.y < 255)
			{
				if (sprite.src.x < 450) sprite.src.x += 75;
				else {
					sprite.src.x = 0;
					sprite.src.y += 85;
				}
			}
			else
			{
				if (sprite.src.x < 375) sprite.src.x += 75;
				else {
					sprite.src.x = 0;
					sprite.src.y = 0;
				}
			}
		}
	}

	void close() {
		// garbage pickup
		delete text;
		delete surface;
	}
};