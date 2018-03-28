#pragma once
#include "SDL.h"
#include "Sprite.h"

class Physics {
protected:
	// varibles for timing the physics
	float vX, vY;					// velocity
	float aX, aY;					// acceleration
	float last, dt;
public:
	Physics()
	{
		vX = 100.0;
		vY = 100.0;
		aX = 100.0;
		aY = 100.0;
		dt = 0.0;
		last = 0.0;
	}

	void runPhysics(Sprite sprite, float tempDest, SDL_Scancode keystate) {
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

	void updateMovement(float &pVar, float &vVar, float &aVar, float dt)
	{
		vVar = vVar + (aVar * dt);
		pVar = pVar + (vVar * dt);
	}

	void swapNegatives(float &vVar, float &aVar)
	{
		vVar = -vVar;
		aVar = -aVar;
	}

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
		if (sprite.getdest().x > (640 - 80))	// check if touch right side of screen
		{
			sprite.setDestX(640 - 81);
		}
	}

	void setDT() { dt = ((float)SDL_GetTicks() - last) / (float)1000.0; }
	void setLast() { last = SDL_GetTicks(); }
};