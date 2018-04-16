#define DEBUG(s) { std::cout << s << std::endl; }



#pragma once
#include "SDL.h"
#include "Physics.h"
#include "Render.h"

using namespace std;

// Physics functions at bottom

class Sprite : public Physics {
protected:
	Render renderer;
	SDL_Texture* motion = NULL;
	SDL_Texture* stand = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect src, dest,standSrc,standDest; // dest = sprite pos / game sprite
	bool right = true;
	int health;

public:
	Sprite()
	{
	}

	void setDestX(float x) { dest.x = x; }
	void setDestY(float y) { dest.y = y; }
	void setSrcX(float x) { src.x = x; }
	void setSrcY(float y) { src.y = y; }
	void setStandSrcX(float x) { standSrc.x = x; }
	void setStandDestX(float x) { standDest.x = x; }

	void setHealth(int x) { health = x; }

	SDL_Rect getsrc() { return this->src; }
	SDL_Rect getdest() { return this->dest; }
	SDL_Rect getStandSrc() { return this->standSrc; }
	SDL_Rect getStandDest() { return this->standDest; }

	SDL_Surface* getSurface() { return this->surface; }
	SDL_Texture* getSpriteMotionTexture() { return this->motion; }
	SDL_Texture* getSpriteStandTexture() { return this->stand; }

	bool isfacingright() { return right; }

	int getHealth() { return health; }

	Sprite createSprite(SDL_Renderer* renderer, int srcx, int srcy, int destx, int desty)
	{
		Sprite newS;
		newS.src = { 0, 0, srcx, srcy };
		newS.standSrc = { 0,0,91, 150 };
		newS.dest.x = destx; newS.dest.y = desty; newS.dest.h = 75; newS.dest.w = 80;
		newS.standDest = { destx,desty,60,75 };
		surface = SDL_LoadBMP("duderighttest.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 162, 232));
		newS.motion = SDL_CreateTextureFromSurface(renderer, surface);
		surface = SDL_LoadBMP("dudestand.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 162, 232));
		newS.stand = SDL_CreateTextureFromSurface(renderer, surface);

		return newS;
	}

	void walkingAnimate(SDL_Texture*barImg, SDL_Rect bar, SDL_Rect barSrc, SDL_Texture* bg, Sprite sprite)
	{
		// function to put the animation stuff in
		float tempSrcX;
		float tempSrcY;
		renderer.renderBg(bg);
		renderer.renderHudObject(barImg, barSrc,bar);
		renderer.renderSprite(sprite.getSpriteMotionTexture(), sprite.isfacingright(), sprite.getsrc(), sprite.getdest());
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

	void move(SDL_Texture* barImg, SDL_Rect bar, SDL_Rect barSrc,SDL_Texture* bg, Sprite sprite, SDL_Scancode keystate)
	{
		float tempDest;
		if (sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			// if sprite is facing right and moves right
			walkingAnimate(barImg,bar,barSrc,bg, sprite);		// will work on this
			right = true;
			tempDest = sprite.dest.x;
			//runPhysics(sprite, tempDest, keystate);
			tempDest += 3;
			setDestX(tempDest);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			// if sprite is facing left and moves left
			walkingAnimate(barImg, bar, barSrc, bg, sprite);
			right = false;
			tempDest = sprite.dest.x;
			//runPhysics(sprite, tempDest, keystate);	// this causes him to moonwalk?? IDK.
			tempDest -= 3;
			setDestX(tempDest);
		}
		else if (sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			// if sprite is facing right and moves left
			walkingAnimate(barImg, bar, barSrc, bg, sprite);
			right = false;
			tempDest = sprite.dest.x;
			//runPhysics(sprite, tempDest, keystate);
			tempDest -= 3;
			setDestX(tempDest);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			// if sprite is facing left and moves right
			walkingAnimate(barImg, bar, barSrc, bg, sprite);
			right = true;
			tempDest = sprite.dest.x;
			//runPhysics(sprite, tempDest, keystate);
			tempDest += 3;
			//DEBUG(tempDest);
			setDestX(tempDest);
		}
	}

	void jump(Sprite sprite)
	{
		float tempY = sprite.dest.y;
		if (tempY >= 175) // upwards part of jump
		{
			tempY -= 8;
			setDestY(tempY);
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
	}

	void drop(Sprite sprite)
	{
		float tempY = sprite.dest.y;
		tempY += 8;
		setDestY(tempY);
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

	void runPhysics(Sprite &sprite, float &tempDest, SDL_Scancode keystate) {
		if (sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			if (vX < 0) swapNegatives(vX, aX);
			setLast();
			tempDest = updateMovement(tempDest, vX, aX, dt);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT) // if sprite is facing left and moves left
		{
			if (vX > 0)	swapNegatives(vX, aX);
			setLast();
			tempDest = updateMovement(tempDest, vX, aX, dt);
		}
		else if (sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT) // if sprite is facing right and moves left
		{
			if (vX > 0) swapNegatives(vX, aX);
			setLast();
			tempDest = updateMovement(tempDest, vX, aX, dt);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT) // if sprite is facing left and moves right;
		{
			if (vX < 0) swapNegatives(vX, aX);
			setLast();
			tempDest = updateMovement(tempDest, vX, aX, dt);
		}
	}
};
