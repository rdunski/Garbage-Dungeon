#pragma once
#include "SDL.h"
#include "Physics.h"
#include "Render.h"

using namespace std;

//Physics function at bottom

class Sprite : Physics {
protected:
	Render renderer;
	SDL_Texture * text = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect src;  // src = sprite sheet
	SDL_Rect dest; // dest = sprite pos / game sprite
	bool right = true;
public:
	Sprite()
	{
	}
	void setDestX(float x) { dest.x = x; }
	void setDestY(float y) { dest.y = y; }
	void setSrcX(float x) { src.x = x; }
	void setSrcY(float y) { src.y = y; }

	SDL_Rect getsrc() { return this->src; }
	SDL_Rect getdest() { return this->dest; }

	SDL_Surface* getSurface() { return this->surface; }
	SDL_Texture* getSpriteTexture() { return this->text; }

	bool isfacingright() { return right; }

	Sprite createSprite(SDL_Renderer* renderer, string filename,
		int srcx, int srcy, int destx, int desty)
	{
		Sprite newS;
		newS.src = { 0, 0, srcx, srcy };
		newS.dest.x = destx; newS.dest.y = desty; newS.dest.h = 75; newS.dest.w = 80;
		surface = SDL_LoadBMP("duderight.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 162, 232));
		newS.text = SDL_CreateTextureFromSurface(renderer, surface);

		return newS;
	}

	void walkingAnimate(SDL_Texture*barImg, SDL_Rect bar, SDL_Texture* bg, Sprite sprite)
	{
		float tempSrcX;
		float tempSrcY;
		// function to put the animation stuff in
		renderer.renderBg(bg);
		renderer.renderHudObject(barImg, bar);
		renderer.renderSprite(sprite.getSpriteTexture(), sprite.isfacingright(), sprite.getsrc(), sprite.getdest());
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
		tempSrcX = sprite.src.x;
		tempSrcY = sprite.src.y;
		setSrcX(tempSrcX);
		setSrcY(tempSrcY);
	}

	void move(SDL_Texture* barImg, SDL_Rect bar, SDL_Texture* bg, Sprite sprite, SDL_Scancode keystate)
	{
		float tempDest;
		if (sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			//if sprite is facing right and moves right
			
			walkingAnimate(barImg,bar,bg, sprite);		// will work on this
			right = true;
			tempDest = sprite.dest.x;
			tempDest += 3;
			setDestX(tempDest);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			//if sprite is facing left and moves left
			
			walkingAnimate(barImg, bar, bg, sprite);
			right = false;
			tempDest = sprite.dest.x;
			tempDest -= 3;
			setDestX(tempDest);
		}
		else if (sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			//if sprite is facing right and moves left
			walkingAnimate(barImg, bar, bg, sprite);
			right = false;
			tempDest = sprite.dest.x;
			tempDest -= 3;
			setDestX(tempDest);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			//if sprite is facing left and moves right
			walkingAnimate(barImg,bar,bg, sprite);
			right = true;
			tempDest = sprite.dest.x;
			tempDest += 3;
			setDestX(tempDest);
		}
	}

	void jump(SDL_Renderer* renderer, SDL_Texture* bg, Sprite sprite)
	{

		// Will work on physics
		bool complete = false;
		float tempX;
		while (!complete) // upwards part of jump
		{
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			if(sprite.isfacingright())
				SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			else
				SDL_RenderCopyEx(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest,NULL,NULL,SDL_FLIP_HORIZONTAL);
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
			if (sprite.isfacingright())
				SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
			else
				SDL_RenderCopyEx(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest, NULL, NULL, SDL_FLIP_HORIZONTAL);
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

	/* ---------------------BEGIN PHYSICS---------------------
	---------------------------------------------------------------
	---------------------------------------------------------------
	---------------------------------------------------------------*/

	void checkPosition(Sprite sprite) {
		if (sprite.getdest().y > 275)	// check if under the ground
		{
			if (aX < 0)
			{
				aY = -aY;
			}
		}
		if (sprite.getdest().y <= 0)	// check if above screen
		{
			sprite.setDestY(75);
		}
		if (sprite.getdest().y < 275)	// check if above ground
		{
			vY = -400;
		}
		if (sprite.getdest().x < 0)		// check if touch left side of screen
		{
			sprite.setDestX(1);
		}
		if (sprite.getdest().x >(640 - 80))	// check if touch right side of screen
		{
			sprite.setDestX(640 - 81);
		}
	}

	void runPhysics(Sprite &sprite, float tempDest, SDL_Scancode keystate) {
		if (sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			if (vX < 0) swapNegatives(vX, aX);
			setLast();
			updateMovement(tempDest, vX, aX, dt);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT) // if sprite is facing left and moves left
		{
			if (vX > 0)	swapNegatives(vX, aX);
			setLast();
			updateMovement(tempDest, vX, aX, dt);
		}
		else if (sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT) // if sprite is facing right and moves left
		{
			if (vX > 0) swapNegatives(vX, aX);
			setLast();
			updateMovement(tempDest, vX, aX, dt);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT) // if sprite is left and moves right;
		{
			if (vX < 0) swapNegatives(vX, aX);
			setLast();
			updateMovement(tempDest, vX, aX, dt);
		}
	}
};
