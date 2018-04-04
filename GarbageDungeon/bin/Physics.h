#pragma once
#include "SDL.h"

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

	virtual void updateMovement(float &pVar, float &vVar, float &aVar, float dt)
	{
		vVar = vVar + (aVar * dt);
		pVar = pVar + (vVar * dt);
	}

	virtual void swapNegatives(float &vVar, float &aVar)
	{
		vVar = -vVar;
		aVar = -aVar;
	}

	virtual void setDT() { dt = ((float)SDL_GetTicks() - last) / (float)1000.0; }
	virtual void setLast() { last = SDL_GetTicks(); }
	virtual ~Physics() { }
};