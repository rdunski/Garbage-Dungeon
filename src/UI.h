#pragma once

#include "SDL.h"
#include "Object.h"
#include "Render.h"
#include "Sprite.h"

class UI {
protected:
	Object healthBar;
	int SCREEN_WIDTH = 640;											// initial screen width, CAN NOW CHANGE
	int SCREEN_HEIGHT = 480;										// inital screen height, CAN NOW CHANGE
	SDL_Texture* bg = NULL;											// background
	SDL_Texture* pause = NULL;
	SDL_Texture* revive = NULL;										// revive/dead background
	SDL_Surface* surface = NULL;									// surface for assigning textures

public:

	float getScreenHeight() { return this->SCREEN_HEIGHT; }
	float getScreenWidth() { return this->SCREEN_WIDTH; }

	void setScreenWidth(int width) { SCREEN_WIDTH = width; }
	void setScreenHeight(int height) { SCREEN_HEIGHT = height; }

	SDL_Texture *getBackground() { return this->bg; }
	SDL_Texture *getPause() { return this->pause; }
	SDL_Texture *getRevive() { return this->revive; }
	SDL_Surface *getSurface() { return this->surface; }
	Object getBar() { return this->healthBar; }

	void updateBarX(int x) { healthBar.setObjectSrcX(x); }
	void updateBarY(int y) { healthBar.setObjectSrcY(y); }

	void setBG(Render renderer)
	{
		surface = SDL_LoadBMP("images/Forest0.bmp");
		bg = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	}

	void setRevive(Render renderer)
	{
		surface = SDL_LoadBMP("images/revive.bmp");
		revive = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	}

	void setPause(Render renderer)
	{
		surface = SDL_LoadBMP("images/pause.bmp");
		pause = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
		SDL_SetTextureAlphaMod(pause, 200);
	}

	void checkWindowPos(Sprite &sprite) // "scene switching" (basically reverts position based on screen edge collision)
	{
		if (sprite.getdest().x > (getScreenWidth()) + sprite.getdest().w - (getScreenWidth()*.03125))
			sprite.setDestX(-sprite.getdest().w + (getScreenWidth()*.03125));

		if (sprite.getdest().x < (-sprite.getdest().w + (getScreenWidth()*.03125)))
			sprite.setDestX((getScreenWidth() + sprite.getdest().w) - (getScreenWidth()*.03125));
	}

	void setBar(Render renderer)
	{
		healthBar.setObjectDest((getScreenWidth()*.015625), (getScreenHeight()*.9375),
			(getScreenWidth()*.234375), (getScreenHeight()*.041666));
		healthBar.setObjectSrc(0, 0, 75, 10);
		surface = SDL_LoadBMP("images/health_bar_sheet.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
		healthBar.setImage(healthBar.createImage(surface, renderer));
	}

	void updateWin(Render renderer)	// if the window is rescaled
	{
		setScreenHeight(SDL_GetWindowSurface(renderer.getWindow())->h);
		setScreenWidth(SDL_GetWindowSurface(renderer.getWindow())->w);
		// do we call jump here?
	}

	void checkHealth(Sprite sprite)
	{
		if (sprite.getHealth() < 0)		// after becoming a zombie, set health to full
			sprite.setHealth(100);

		int hp = sprite.getHealth();	// variable for switch

		switch (hp)						// update health bar based on current health
		{
		case 100:
			updateBarX(0);
			updateBarY(0);
			break;
		case 90:
			updateBarX(0);
			updateBarY(11);
			break;
		case 80:
			updateBarX(0);
			updateBarY(22);
			break;
		case 70:
			updateBarX(0);
			updateBarY(33);
			break;
		case 60:
			updateBarX(0);
			updateBarY(44);
			break;
		case 50:
			updateBarX(80);
			updateBarY(0);
			break;
		case 40:
			updateBarX(80);
			updateBarY(11);
			break;
		case 30:
			updateBarX(80);
			updateBarY(22);
			break;
		case 20:
			updateBarX(80);
			updateBarY(33);
			break;
		case 10:
			updateBarX(80);
			updateBarY(44);
			break;
		case 0:
			updateBarX(80);
			updateBarY(55);
			break;
		}
	}
};