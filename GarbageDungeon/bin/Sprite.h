#define DEBUG(s) { std::cout << s << std::endl; }

#pragma once
#include <iostream>
#include "SDL.h"
#include "Physics.h"
#include "Render.h"

using namespace std;
// Physics functions at bottom

class Sprite : public Physics {
protected:
	bool right = true;
	int health;
	Render renderer;
	SDL_Texture* motion = NULL;
	SDL_Texture* stand = NULL;
	SDL_Texture* jump = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect src, dest,standSrc,standDest,jumpSrc,jumpDest; // dest = sprite pos / game sprite

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
	void setStandDestY(float y) { standDest.y = y; }
	void setJumpSrcX(float x) { jumpSrc.x = x; }
	void setJumpDestX(float x) { jumpDest.x = x; }
	void setJumpDestY(float y) { jumpDest.y = y; }
	void setHealth(int x) { health = x; }
	void setDestScale(int w, int h) { dest.w = w; dest.h = h; }
	void setStandDestScale(int w, int h) { standDest.w = w; standDest.h = h; }
	void setJumpDestScale(int w, int h) { jumpDest.w = w; jumpDest.h = h; }

	SDL_Rect getsrc() { return this->src; }
	SDL_Rect getdest() { return this->dest; }
	SDL_Rect getStandSrc() { return this->standSrc; }
	SDL_Rect getStandDest() { return this->standDest; }
	SDL_Rect getJumpSrc() { return this->jumpSrc; }
	SDL_Rect getJumpDest() { return this->jumpDest; }

	int getHealth() { return health; }

	bool isfacingright() { return right; }

	SDL_Surface* getSurface() { return this->surface; }

	SDL_Texture* getSpriteMotionTexture() { return this->motion; }
	SDL_Texture* getSpriteStandTexture() { return this->stand; }
	SDL_Texture* getSpriteJumpTexture() { return this->jump; }

	Sprite createSprite(SDL_Renderer* renderer, int destx, int desty, int screen_height, int screen_width)
	{
		Sprite newS;
		newS.src = { 0, 0, 75, 80 };
		newS.dest = { destx,desty,(static_cast<int>(screen_width*.125)),(static_cast<int>(screen_height*.15625)) };
		newS.standSrc = { 0,0,91, 150 };
		newS.standDest = { destx,desty,(static_cast<int>(screen_width*.09375)), (static_cast<int>(screen_height*.15625)) };
		newS.jumpSrc = { 0,0,121,143 };
		newS.jumpDest = { destx, desty, (static_cast<int>(screen_width*.125)), (static_cast<int>(screen_height*.15625)) };
		surface = SDL_LoadBMP("duderighttest.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 162, 232));
		newS.motion = SDL_CreateTextureFromSurface(renderer, surface);
		surface = SDL_LoadBMP("dudestand.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 162, 232));
		newS.stand = SDL_CreateTextureFromSurface(renderer, surface);
		surface = SDL_LoadBMP("dudejump.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 162, 232));
		newS.jump = SDL_CreateTextureFromSurface(renderer, surface);

		return newS;
	}

	void updateSprite(Sprite sprite, int destx, int desty, int screen_height, int screen_width)
	{
		sprite.setDestX(destx);
		sprite.setDestY(desty);
		sprite.setDestScale((static_cast<int>(screen_width*.125)), (static_cast<int>(screen_height*.15625)));
		sprite.setStandDestScale((static_cast<int>(screen_width*.09375)), (static_cast<int>(screen_height*.15625)));
		sprite.setJumpDestScale((static_cast<int>(screen_width*.125)), (static_cast<int>(screen_height*.15625)));
	}

	void walkingAnimate(Sprite sprite)
	{
		// function to put the animation stuff in
		float tempSrcX;
		float tempSrcY;
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

	void move(Sprite sprite, SDL_Scancode keystate, int screen_height, int screen_width)
	{
		float tempDest;
		if (sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			// if sprite is facing right and moves right
			walkingAnimate(sprite);		// will work on this
			right = true;
			tempDest = sprite.dest.x;
			//runPhysics(sprite, tempDest, keystate);
			tempDest += (screen_width*.005);
			setDestX(tempDest);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			// if sprite is facing left and moves left
			walkingAnimate(sprite);
			right = false;
			tempDest = sprite.dest.x;
			//runPhysics(sprite, tempDest, keystate);	// this causes him to moonwalk?? IDK.
			tempDest -= (screen_width*.005);
			setDestX(tempDest);
		}
		else if (sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT)
		{
			// if sprite is facing right and moves left
			walkingAnimate(sprite);
			right = false;
			tempDest = sprite.dest.x;
			//runPhysics(sprite, tempDest, keystate);
			tempDest -= (screen_width*.005);
			setDestX(tempDest);
		}
		else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT)
		{
			// if sprite is facing left and moves right
			walkingAnimate(sprite);
			right = true;
			tempDest = sprite.dest.x;
			//runPhysics(sprite, tempDest, keystate);
			tempDest += (screen_width*.005);
			//DEBUG(tempDest);
			setDestX(tempDest);
		}
	}

	void beginJump(Sprite sprite, bool moving, int screen_height, int screen_width)
	{
		float tempY = sprite.dest.y;
		float tempX = sprite.dest.x;

		if (sprite.jumpSrc.x < 968 && sprite.jumpSrc.x != 0)
			setJumpSrcX(getJumpSrc().x + 121);
		tempY -= (screen_height*.016666);
		setDestY(tempY);
		if (moving && sprite.isfacingright())
		{
			tempX += (screen_width*.002);
			setDestX(tempX);
		}
		if (moving && !sprite.isfacingright())
		{
			tempX -= (screen_width*.002);
			setDestX(tempX);
		}
	}

	void drop(Sprite sprite, bool moving, int screen_height, int screen_width) //downwards part of jump
	{
		float tempY = sprite.dest.y;
		float tempX = sprite.dest.x;
		tempY += (screen_height*.016666);
		setDestY(tempY);
		if (moving && sprite.isfacingright())
		{
			tempX += (screen_width*.001);
			setDestX(tempX);
		}
		if (moving && !sprite.isfacingright())
		{
			tempX -= (screen_width*.001);
			setDestX(tempX);
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
