#pragma once
#include "SDL.h"
#include "Game.h"
using namespace std;

class Sprite {
protected:
	SDL_Texture * text = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect src;  // src = sprite sheet
	SDL_Rect dest; // dest = sprite pos
									// changeable includes x, y (instead of pX, pY)
	bool right = true;
	// varibles for timing the physics
	float vX, vY;					// velocity
	float aX, aY;					// acceleration
	float last, dt; 

	// will be moving filename here, creating 
	// getter/setter for switchdirection function

public:
	Sprite()
	{
	}

	
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

	SDL_Surface* getSurface() { return this->surface; }
	SDL_Texture* getSpriteTexture() { return this->text; }
	SDL_Rect getsrc() { return this->src; }
	SDL_Rect getdest() { return this->dest; }
	bool isfacingright() { return right; }

	void setDestX(float x) { dest.x = x; }
	void setDestY(float y) { dest.y = y; }
	void setSrcX(float x) { src.x = x; }
	void setSrcY(float y) { src.y = y; }

	void walkingAnimate(SDL_Renderer* renderer, SDL_Texture* bg, Sprite sprite)
	{
		// function to put the animation stuff in
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
			//sprite.walkingAnimate(renderer, bg, sprite);		// will work on this
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
			SDL_RenderCopyEx(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest,NULL,NULL,SDL_FLIP_HORIZONTAL);
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
			SDL_RenderCopyEx(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest, NULL, NULL, SDL_FLIP_HORIZONTAL);
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
			tempSrcX = sprite.src.x;
			tempSrcY = sprite.src.y;
			setSrcX(tempSrcX);
			setSrcY(tempSrcY);
			tempDest = sprite.dest.x;
			tempDest += 3;
			setDestX(tempDest);
		}
		/*SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
		SDL_RenderPresent(renderer);*/
	}

	//void move(SDL_Renderer* renderer, SDL_Texture* bg, Sprite sprite, SDL_Scancode keystate)
	//{
	//	float tempDest;
	//	float tempSrcX;
	//	float tempSrcY;
	//	if (sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT) // if sprite is facing right and moves right
	//	{
	//		sprite.walkingAnimate(renderer, bg, sprite);
	//		right = true;
	//		tempDest = sprite.dest.x;
	//		tempSrcX = sprite.src.x;
	//		tempSrcY = sprite.src.y;
	//		setSrcX(tempSrcX);
	//		setSrcY(tempSrcY);
	//		if (vX < 0) swapNegatives(vX, aX);
	//		sprite.setLast();
	//		sprite.updateMovement(tempDest, sprite.vX, sprite.aX, dt);
	//		setDestX(tempDest);
	//	}
	//	else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT) // if sprite is facing left and moves left
	//	{
	//		sprite.walkingAnimate(renderer, bg, sprite);
	//		right = false;
	//		tempDest = sprite.dest.x;
	//		tempSrcX = sprite.src.x;
	//		tempSrcY = sprite.src.y;
	//		setSrcX(tempSrcX);
	//		setSrcY(tempSrcY);
	//		if (vX > 0)	swapNegatives(vX, aX);
	//		sprite.setLast();
	//		sprite.updateMovement(tempDest, sprite.vX, sprite.aX, dt);
	//		setDestX(tempDest);
	//	}
	//	else if (sprite.isfacingright() && keystate == SDL_SCANCODE_LEFT) // if sprite is facing right and moves left
	//	{
	//		sprite.walkingAnimate(renderer, bg, sprite);
	//		right = false;
	//		switchDirection(sprite, renderer);
	//		tempDest = sprite.dest.x;
	//		tempSrcX = sprite.src.x;
	//		tempSrcY = sprite.src.y;
	//		setSrcX(tempSrcX);
	//		setSrcY(tempSrcY);
	//		if (vX > 0) swapNegatives(vX, aX);
	//		sprite.setLast();
	//		sprite.updateMovement(tempDest, sprite.vX, sprite.aX, dt);
	//		setDestX(tempDest);
	//	}
	//	else if (!sprite.isfacingright() && keystate == SDL_SCANCODE_RIGHT) // if sprite is left and moves right;
	//	{
	//		sprite.walkingAnimate(renderer, bg, sprite);
	//		right = true;
	//		switchDirection(sprite, renderer);
	//		tempDest = sprite.dest.x;
	//		tempSrcX = sprite.src.x;
	//		tempSrcY = sprite.src.y;
	//		setSrcX(tempSrcX);
	//		setSrcY(tempSrcY);
	//		if (vX < 0) swapNegatives(vX, aX);
	//		sprite.setLast();
	//		sprite.updateMovement(tempDest, sprite.vX, sprite.aX, dt);
	//		setDestX(tempDest);
	//	}
	//	SDL_RenderCopy(renderer, bg, NULL, NULL);
	//	SDL_RenderCopy(renderer, sprite.getSpriteTexture(), &sprite.src, &sprite.dest);
	//	SDL_RenderPresent(renderer);
	//}

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

	void close()
	{
		// garbage pickup
		delete text;
		delete surface;
	}
};
