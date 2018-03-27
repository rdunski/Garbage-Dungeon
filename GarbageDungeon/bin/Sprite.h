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
	SDL_Texture* text = NULL;
	SDL_Texture* spRight = NULL;	// texture for loading in the right-facing spring sheet to quickly reference it.
	SDL_Texture* spLeft = NULL;		// texture for loading in the left-facing spring sheet to quickly reference it.
	SDL_Surface* surface = NULL;
	SDL_Rect src;  // src is manipulating sprite sheet
	SDL_Rect dest; // dest is sprite position on screen
		// changeable includes x, y (instead of pX, pY)
	bool right = true;
	float vX, vY;	// velocity
	float aX, aY;	// acceleration
	float last, dt; // varibles for timing the physics

	// will be moving filename here, creating 
	// getter/setter for switchdirection function

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
		directionInit(renderer);
		if (filename == "duderight.bmp") newS.text = spRight;
		else if (filename == "dudeleft.bmp") newS.text = spLeft;

		return newS;
	}

	SDL_Surface* getSurface() { return this->surface; }
	SDL_Texture* getSpriteTexture() { return this->text; }
	SDL_Rect getsrc() { return this->src; }
	SDL_Rect getdest() { return this->dest; }
	void setDestX(float x) { dest.x = x; }
	void setDestY(float y) { dest.y = y; }
	void setSrcX(float x) { src.x = x; }
	void setSrcY(float y) { src.y = y; }
	bool isfacingright() { return right; }

	void directionInit(SDL_Renderer* renderer) {
		// function for creating the textures that will hold the left
		// and right facing sprite sheets

		// left
		surface = SDL_LoadBMP("dudeleft.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
		spLeft = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		// right
		surface = SDL_LoadBMP("duderight.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
		spRight = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	void switchDirection(Sprite sprite, SDL_Renderer* renderer)
	{
		// if sprite is facing one direction and the other is called, 
		// should assign current texture to the opposite
		if (sprite.isfacingright() && SDLK_LEFT)
		{
			text = spLeft;
			sprite.right = false;
		}
		else if (!sprite.isfacingright() && SDLK_RIGHT)
		{
			text = spRight;
			sprite.right = true;
		}
	}

	void move(SDL_Renderer* renderer, SDL_Texture* bg, Sprite sprite, SDL_Scancode keystate)
	{
		float tempDest;
		float tempSrcX;
		float tempSrcY;
		if (sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)//if sprite is facing right and moves right
		{
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			SDL_RenderPresent(renderer);
			if (sprite.src.y < 255)
			{
				if (sprite.src.x < 450)
				{
					sprite.src.x += 75;

				}
				else
				{
					sprite.src.x = 0;
					sprite.src.y += 85;
				}
			}
			else
			{
				if (sprite.src.x < 375) sprite.src.x += 75;
				else
				{
					sprite.src.x = 0;
					sprite.src.y = 0;
				}
			}
			right = true;
			tempSrcX = sprite.src.x;
			tempSrcY = sprite.src.y;
			setSrcX(tempSrcX);
			setSrcY(tempSrcY);
			tempDest = sprite.dest.x;
			tempDest += 3;
			setDestX(tempDest);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			//if sprite is facing left and moves left
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			SDL_RenderPresent(renderer);
			if (sprite.src.y < 255)
			{
				if (sprite.src.x < 450)
				{
					sprite.src.x += 75;

				}
				else
				{
					sprite.src.x = 0;
					sprite.src.y += 85;
				}
			}
			else
			{
				if (sprite.src.x < 375) sprite.src.x += 75;
				else
				{
					sprite.src.x = 0;
					sprite.src.y = 0;
				}
			}
			right = false;
			tempSrcX = sprite.src.x;
			tempSrcY = sprite.src.y;
			setSrcX(tempSrcX);
			setSrcY(tempSrcY);
			tempDest = sprite.dest.x;
			tempDest -= 3;
			setDestX(tempDest);
		}
		else if (sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			//if sprite is facing right and moves left
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			SDL_RenderPresent(renderer);
			if (sprite.src.y < 255)
			{
				if (sprite.src.x < 450)
				{
					sprite.src.x += 75;

				}
				else
				{
					sprite.src.x = 0;
					sprite.src.y += 85;
				}
			}
			else
			{
				if (sprite.src.x < 375) sprite.src.x += 75;
				else
				{
					sprite.src.x = 0;
					sprite.src.y = 0;
				}
			}
			right = false;
			switchDirection(sprite, renderer);
			tempSrcX = sprite.src.x;
			tempSrcY = sprite.src.y;
			setSrcX(tempSrcX);
			setSrcY(tempSrcY);
			tempDest = sprite.dest.x;
			tempDest -= 3;
			setDestX(tempDest);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{

			//if sprite is left and moves right;
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			SDL_RenderPresent(renderer);
			if (sprite.src.y < 255)
			{
				if (sprite.src.x < 450)
				{
					sprite.src.x += 75;

				}
				else
				{
					sprite.src.x = 0;
					sprite.src.y += 85;
				}
			}
			else
			{
				if (sprite.src.x < 375) sprite.src.x += 75;
				else
				{
					sprite.src.x = 0;
					sprite.src.y = 0;
				}
			}
			right = true;
			switchDirection(sprite, renderer);
			tempSrcX = sprite.src.x;
			tempSrcY = sprite.src.y;
			setSrcX(tempSrcX);
			setSrcY(tempSrcY);
			tempDest = sprite.dest.x;
			tempDest += 3;
			setDestX(tempDest);
		}

		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
		SDL_RenderPresent(renderer);

	}

	void jump(SDL_Renderer* renderer, SDL_Texture* bg, Sprite sprite)
	{

		// This whole function should be scrapped for one using correct physics. 
		// Will be more complicated than simple movement, so it should probably 
		// stay in it's own function
		bool complete = false;
		float tempX;
		while (!complete) // upwards part of jump
		{
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			SDL_RenderPresent(renderer);

			if (sprite.dest.y < 175)
			{
				complete = true;
				break;
			}
			if (sprite.isfacingright())
				sprite.dest.x += 6;
			else
				sprite.dest.x -= 6;
			sprite.dest.y -= 8;
			SDL_Delay(1000 / 24); // set framerate to 24 FPS
			SDL_RenderClear(renderer);

			if (sprite.src.y < 255)
			{
				if (sprite.src.x < 450) sprite.src.x += 75;
				else 
				{
					sprite.src.x = 0;
					sprite.src.y += 85;
				}
			}
			else
			{
				if (sprite.src.x < 375) sprite.src.x += 75;
				else 
				{
					sprite.src.x = 0;
					sprite.src.y = 0;
				}
			}
		}
		while (complete) // downwards part of jump
		{
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			SDL_RenderPresent(renderer);

			if (sprite.dest.y >= 275)
			{
				complete = false;
				break;
			}
			if (sprite.isfacingright())
				sprite.dest.x += 5;
			else
				sprite.dest.x -= 5;
			sprite.dest.y += 10;
			SDL_Delay(1000 / 24);
			SDL_RenderClear(renderer);

			if (sprite.src.y < 255)
			{
				if (sprite.src.x < 450) sprite.src.x += 75;
				else 
				{
					sprite.src.x = 0;
					sprite.src.y += 85;
				}
			}
			else
			{
				if (sprite.src.x < 375) sprite.src.x += 75;
				else 
				{
					sprite.src.x = 0;
					sprite.src.y = 0;
				}
			}
		}
		tempX = sprite.dest.x;
		setDestX(tempX);
	}

	void close() 
	{
		// garbage pickup
		delete text;
		delete surface;
	}
};