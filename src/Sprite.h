#define DEBUG(s) { std::cout << s << std::endl; }

#pragma once
#include <iostream>
#include "SDL.h"
#include "Physics.h"
#include "Render.h"
#include "Sound.h"

using namespace std;
// Physics functions at bottom

class Sprite : public Physics {
protected:
	bool right = true;										// boolean to check if sprite is facing right or not
	int health;												// health variable
	Render renderer;										// the renderer...
	SDL_Texture* motion = NULL;								// moving texture
	SDL_Texture* stand = NULL;								// standing texture
	SDL_Texture* jump = NULL;								// jumping texture
	SDL_Surface* surface = NULL;							// placeholder surface for intializing
	SDL_Rect src, dest,standSrc,standDest,jumpSrc,jumpDest; // source and destination rects for the various textures

public:
	Sprite()
	{
	}

	// Setters (may need cleaning up)
	void setDestX(float x) { this->dest.x = x; }
	void setDestY(float y) { this->dest.y = y; }
	void setSrcX(float x) { this->src.x = x; }
	void setSrcY(float y) { this->src.y = y; }
	void setStandSrcX(float x) { this->standSrc.x = x; }
	void setStandDestX(float x) { this->standDest.x = x; }
	void setStandDestY(float y) { this->standDest.y = y; }
	void setJumpSrcX(float x) { this->jumpSrc.x = x; }
	void setJumpDestX(float x) { this->jumpDest.x = x; }
	void setJumpDestY(float y) { this->jumpDest.y = y; }
	void setHealth(int x) { this->health = x; }
	void setDestScale(int w, int h) { this->dest.w = w; this->dest.h = h; }
	void setStandDestScale(int w, int h) { this->standDest.w = w; this->standDest.h = h; }
	void setJumpDestScale(int w, int h) { this->jumpDest.w = w; this->jumpDest.h = h; }

	// Getters
	SDL_Rect getsrc() { return this->src; }
	SDL_Rect getdest() { return this->dest; }
	SDL_Rect getStandSrc() { return this->standSrc; }
	SDL_Rect getStandDest() { return this->standDest; }
	SDL_Rect getJumpSrc() { return this->jumpSrc; }
	SDL_Rect getJumpDest() { return this->jumpDest; }

	SDL_Surface* getSurface() { return this->surface; }

	SDL_Texture* getSpriteMotionTexture() { return this->motion; }
	SDL_Texture* getSpriteStandTexture() { return this->stand; }
	SDL_Texture* getSpriteJumpTexture() { return this->jump; }

	int getHealth() { return this->health; }

	// boolean check
	bool isfacingright() { return this->right; }

	void initCarlResources(SDL_Renderer* renderer, int destx, int desty, int screen_height, int screen_width)
	{
		this->surface = SDL_LoadBMP("images/duderight.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(this->surface->format, 0, 162, 232));
		this->motion = SDL_CreateTextureFromSurface(renderer, this->surface);

		this->surface = SDL_LoadBMP("images/dudestand.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(this->surface->format, 0, 162, 232));
		this->stand = SDL_CreateTextureFromSurface(renderer, this->surface);

		this->surface = SDL_LoadBMP("images/dudejump.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(this->surface->format, 0, 162, 232));
		this->jump = SDL_CreateTextureFromSurface(renderer, this->surface);

		this->src = { 0, 0, 75, 80 };
		this->dest = { destx,desty,(static_cast<int>(screen_width*.125)),(static_cast<int>(screen_height*.15625)) };
		this->standSrc = { 0,0,91, 150 };
		this->standDest = { destx,desty,(static_cast<int>(screen_width*.09375)), (static_cast<int>(screen_height*.15625)) };
		this->jumpSrc = { 0,0,121,143 };
		this->jumpDest = { destx, desty, (static_cast<int>(screen_width*.125)), (static_cast<int>(screen_height*.15625)) };
	}

	// adjust the sprite width and height to match changing window resolution
	void updateSprite(int screen_height, int screen_width)
	{
		this->setDestScale((static_cast<int>(screen_width*.125)), (static_cast<int>(screen_height*.15625)));
		this->setStandDestScale((static_cast<int>(screen_width*.09375)), (static_cast<int>(screen_height*.15625)));
		this->setJumpDestScale((static_cast<int>(screen_width*.125)), (static_cast<int>(screen_height*.15625)));
	}

	// ANIMATE (shouldn't need any changes/ might break the animation if anything is changed)
	void walkingAnimate(Sprite sprite)
	{
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

	// MOVE (may need cleaning up)
	void move(Sprite sprite, Sound mixer, SDL_Scancode keystate, int screen_height, int screen_width)
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

	// Start jumping!
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

	// Get down!
	void drop(Sprite sprite, bool moving, int screen_height, int screen_width)
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
		if (sprite.getdest().y > 275)		// check if under the ground
		{
			if (aX < 0)
			{
				aY = -aY;
			}
		}
		if (sprite.getdest().y <= 0)		// check if above screen
		{
			sprite.setDestY(75);
		}
		if (sprite.getdest().y < 275)		// check if above ground
		{
			vY = -400;
		}
		if (sprite.getdest().x < 0)			// check if touch left side of screen
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