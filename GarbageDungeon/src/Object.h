#pragma once

#include "SDL.h"
#include "Render.h"

class Object {
protected:
	SDL_Texture* image = NULL;
	SDL_Rect objectDest, objectSrc; // object source rect (position on image) and object destination rect (position on screen)

public:
	Object()
	{
	}

	void setObjectDest(float x, float y, float w, float h) // changing or initializing x,y,w, and h
	{
		this->objectDest.x = x;
		this->objectDest.y = y;
		this->objectDest.w = w;
		this->objectDest.h = h;
	}

	void setObjectSrc(float x, float y, float w, float h) // initialzing x,y,w, and h for source rect 
	{												  	  //(shouldn't need to be modified after first call)
		this->objectSrc.x = x;
		this->objectSrc.y = y;
		this->objectSrc.w = w;
		this->objectSrc.h = h;
	}


	// individual sets for x,y for source and destination rects
	void setObjectDestX(float x) { this->objectDest.x = x; }
	void setObjectDestY(float y) { this->objectDest.y = y; }
	void setObjectSrcX(float x) { this->objectSrc.x = x; }
	void setObjectSrcY(float y) { this->objectSrc.y = y; }

	// used with create image to initalize image (syntax: objectname.setImage(createImage(surface, renderer));  )
	void setImage(SDL_Texture* texture) { this->image = texture; }

	// getters
	SDL_Texture* getImage() { return this->image; }
	SDL_Rect getObjectDest() { return this->objectDest; }
	SDL_Rect getObjectSrc() { return this->objectSrc; }

	// used with set image to initialize image
	SDL_Texture* createImage(SDL_Surface* surface, Render renderer)
	{
		this->image = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
		return this->image;
	}

	// self-expanatory
	void destroyImage() { SDL_DestroyTexture(image); }
};