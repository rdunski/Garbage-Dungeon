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
	SDL_Texture * sprite = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect src;
	SDL_Rect dest;

public:
	Sprite createSprite(SDL_Renderer* renderer, string filename)
	{
		Sprite newS;
		newS.surface = SDL_LoadBMP(filename.c_str());
		SDL_SetColorKey(newS.surface, SDL_TRUE, SDL_MapRGB(newS.surface->format, 0, 0, 0));
		newS.sprite = SDL_CreateTextureFromSurface(renderer, newS.surface);
		return newS;
	}

	SDL_Surface* getSurface() { return this->surface; }
	SDL_Texture* getSprite(){return this->sprite;}
	SDL_Rect getsrc(){ return this->src; }
	SDL_Rect getdest(){ return this->dest; }

	void switchDirection(Sprite &carl, SDL_Texture* &sprite, SDL_Renderer* renderer, bool &right)
	{
		if (right)
		{
			carl.surface = SDL_LoadBMP("dudeleft.bmp");
			SDL_SetColorKey(carl.surface, SDL_TRUE, SDL_MapRGB(carl.surface->format, 0, 0, 0));
			right = false;
		}
		else
		{
			carl.surface = SDL_LoadBMP("duderight.bmp");
			SDL_SetColorKey(carl.surface, SDL_TRUE, SDL_MapRGB(carl.surface->format, 0, 0, 0));
			right = true;
		}

		sprite = SDL_CreateTextureFromSurface(renderer, carl.surface);
	}

	void runright(SDL_Renderer* renderer, SDL_Texture* bg, SDL_Texture* texture, SDL_Rect &bgsrc, SDL_Rect &bgdest)
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopy(renderer, texture, &bgsrc, &bgdest);
		int i = 0;
		SDL_RenderPresent(renderer);
		bgdest.x = bgdest.x + 10;
		SDL_Delay(1000 / 24);
		SDL_RenderClear(renderer);
		if (bgsrc.y < 255) {
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
	void runleft(SDL_Renderer* renderer, SDL_Texture* bg, SDL_Texture* &sprite, SDL_Rect &bgsrc, SDL_Rect &bgdest, bool &right)
	{
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopy(renderer, sprite, &bgsrc, &bgdest);
		SDL_RenderPresent(renderer);
		bgdest.x = bgdest.x - 10;
		SDL_Delay(1000 / 24);
		SDL_RenderClear(renderer);
		if (bgsrc.y < 255) {
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

	void slowwalk(SDL_Renderer* renderer, SDL_Texture* bg, SDL_Texture* sprite, Sprite shield, SDL_Rect bgsrc, SDL_Rect bgdest, bool &done)
	{
		while (bgdest.x != 650) {
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bg, NULL, NULL);
			SDL_RenderCopy(renderer, sprite, &bgsrc, &bgdest);
			SDL_RenderCopy(renderer, shield.getSprite(), &shield.getsrc(), &shield.getdest());
			SDL_RenderPresent(renderer);
			int i = 0;
			bgdest.x = bgdest.x+10;
			SDL_Delay(1000 / 12);
			SDL_RenderClear(renderer);
			if (bgsrc.y < 210) {
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

	void jump(SDL_Renderer* renderer, SDL_Texture* bg, SDL_Texture* sprite, SDL_Rect &bgsrc, SDL_Rect &bgdest, bool &right)
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

	void runleft(SDL_Renderer* renderer, SDL_Texture* bg, SDL_Rect &bgsrc, SDL_Rect &bgdest, bool &right)
	{
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopy(renderer, sprite, &bgsrc, &bgdest);
		SDL_RenderPresent(renderer);
		bgdest.x = bgdest.x - 10;
		SDL_Delay(1000 / 24);
		SDL_RenderClear(renderer);
		if (bgsrc.y < 255) {
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
};