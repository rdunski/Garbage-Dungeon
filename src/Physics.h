#pragma once
#include "SDL.h"

class Physics {
protected:
	float vX, vY;			// velocity
	float aX, aY;			// acceleration
	float last, dt;			// varibles for timing the physics

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

	virtual void checkValues(float &vVar, float &aVar) // make sure velocity and acceleration do not exceed a certain value (prevents Flash-like speed)
	{
		if (abs(vVar) >= 200.0) vVar = 200.0;
		if (abs(aVar) >= 100.0) aVar = 100.0;
	}

	virtual float updateMovement(float &pVar, float &vVar, float &aVar, float dt)
	{
		checkValues(vVar, aVar);
		vVar = vVar + (aVar * dt);
		pVar = pVar + (vVar * dt);
		return pVar;
	}

	virtual void swapNegatives(float &vVar, float &aVar)
	{
		vVar = -vVar;
		aVar = -aVar;
	}

	void setDT() { dt = ((float)SDL_GetTicks() - last) / (float)1000.0; }
	virtual void setLast() { last = SDL_GetTicks(); }

	virtual ~Physics() { }
};