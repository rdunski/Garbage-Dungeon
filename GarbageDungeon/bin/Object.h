#pragma once

#include "SDL.h"
#include "Render.h"

class Object {
protected:
	SDL_Texture * image = NULL;
	SDL_Rect objectDest, objectSrc;
public:
	Object()
	{
	}
	void setInitObjectDest(float x, float y, float w, float h)
	{
		this->objectDest.x = x;
		this->objectDest.y = y;
		this->objectDest.w = w;
		this->objectDest.h = h;
	}

	void setInitObjectSrc(float x, float y, float w, float h)
	{
		this->objectSrc.x = x;
		this->objectSrc.y = y;
		this->objectSrc.w = w;
		this->objectSrc.h = h;
	}

	void setObjectDestX(float x) { this->objectDest.x = x; }
	void setObjectDestY(float y) { this->objectDest.y = y; }
	void setObjectSrcX(float x) { this->objectSrc.x = x; }
	void setObjectSrcY(float y) { this->objectSrc.y = y; }

	void setImage(SDL_Texture* texture) { this->image = texture; }

	SDL_Texture* getImage() { return this->image; }
	SDL_Rect getObjectDest() { return this->objectDest; }
	SDL_Rect getObjectSrc() { return this->objectSrc; }

	SDL_Texture* createImage(SDL_Surface* surface, Render renderer)
	{
		this->image = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
		return this->image;
	}


};