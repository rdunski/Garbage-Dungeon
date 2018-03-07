#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "Game.h"
#include "SDL.h"
using namespace std;

class Sprite {
protected:
	SDL_Texture * text = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect src;
	SDL_Rect dest;
	bool right = true;

public:
	Sprite createSprite(SDL_Renderer* renderer, string filename)
	{
		Sprite newS;
		newS.src = { 0, 0, 75, 80 };
		newS.dest.x = 0; newS.dest.y = 275; newS.dest.h = 75; newS.dest.w = 80;
		newS.surface = SDL_LoadBMP(filename.c_str());
		SDL_SetColorKey(newS.surface, SDL_TRUE, SDL_MapRGB(newS.surface->format, 0, 0, 0));
		newS.text = SDL_CreateTextureFromSurface(renderer, newS.surface);
		return newS;
	}

	SDL_Surface* getSurface() { return this->surface; }
	SDL_Texture* getSpriteTexture(){return this->text;}
	SDL_Rect getsrc(){ return this->src; }
	SDL_Rect getdest(){ return this->dest; }
	bool isfacingright() { return right; }

	void switchDirection(Sprite sprite, SDL_Renderer* renderer)
		//Daniel is working on this now. When the direction facing and direction of action
		//contradict, this will replace the spritesheet in use
	{
		if (sprite.isfacingright() && SDLK_LEFT)
		{
			sprite.surface = SDL_LoadBMP("dudeleft.bmp");
			SDL_SetColorKey(sprite.surface, SDL_TRUE, SDL_MapRGB(sprite.surface->format, 0, 0, 0));
			right = false;
		}
		else if (!sprite.isfacingright() && SDLK_RIGHT)
		{
			sprite.surface = SDL_LoadBMP("duderight.bmp");
			SDL_SetColorKey(sprite.surface, SDL_TRUE, SDL_MapRGB(sprite.surface->format, 0, 0, 0));
			right = true;
		}

		sprite.text = SDL_CreateTextureFromSurface(renderer, sprite.surface);
	}

	void move(SDL_Renderer* renderer, SDL_Texture* bg, Sprite sprite)
	{
		SDL_Rect* src;
		SDL_Rect* dest;
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopy(renderer, getSpriteTexture(), src, dest);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
		//move function should move character left or right, depending on direction facing and user input...
		if (sprite.isfacingright() && SDLK_RIGHT)
		{
			//if sprite is facing right and moves right
		}
		else if (!sprite.isfacingright() && SDLK_LEFT)
		{
			//if sprite is facing left and moves left
		}
		else if (sprite.isfacingright() && SDLK_LEFT)
		{
			//if sprite is facing right and moves left
		}
		else if (!sprite.isfacingright() && SDLK_RIGHT)
		{
			//if sprite is left and moves right
		}
	}

	void jump(SDL_Renderer* renderer, SDL_Texture* bg, SDL_Texture* sprite, SDL_Rect bgsrc, SDL_Rect bgdest, bool &right)
	{
		bool complete = false;
		while (!complete)
		{
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite, &bgsrc, &bgdest);
			SDL_RenderPresent(renderer);

			if (bgdest.y < 175) complete = true;
			bgdest.x += 6;
			bgdest.y -= 8;
			SDL_Delay(1000 / 24);
			SDL_RenderClear(renderer);
			if (bgsrc.y < 255)
			{
				if (bgsrc.x < 450) bgsrc.x += 75;
				else {
					bgsrc.x = 0;
					bgsrc.y += 85;
				}
			}
			else
			{
				if (bgsrc.x < 375) bgsrc.x += 75;
				else {
					bgsrc.x = 0;
					bgsrc.y = 0;
				}
			}
		}						//CREATE ARCH
		while (complete)
		{
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite, &bgsrc, &bgdest);
			SDL_RenderPresent(renderer);
			if (bgdest.y >= 275) complete = false;
			bgdest.x += 5;
			bgdest.y += 10;
			SDL_Delay(1000 / 24);
			SDL_RenderClear(renderer);
			if (bgsrc.y < 255)
			{
				if (bgsrc.x < 450) bgsrc.x += 75;
				else {
					bgsrc.x = 0;
					bgsrc.y += 85;
				}
			}
			else
			{
				if (bgsrc.x < 375) bgsrc.x += 75;
				else {
					bgsrc.x = 0;
					bgsrc.y = 0;
				}
			}
		}
	}
};