#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include "Sprite.h"
#include "Physics.h"
#include "Render.h"
#include "Object.h"
#include "Sound.h"
#include "SDL.h"
using namespace std;

class Game {
protected:
	Render renderer;												// the renderer...
	Object healthBar;												// the health bar
	Sprite carl, evilCarl;											// our guy (also evil Carl)
	Sound mixer;													// da sound machine
	bool quitter, dead, moving, jumping,paused = false;				// boolean checks
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);		// keyboard press events
	int SCREEN_WIDTH = 640;											// initial screen width, CAN NOW CHANGE
	int SCREEN_HEIGHT = 480;										// inital screen height, CAN NOW CHANGE
	int counter = 0;												// counter for jumping
	SDL_DisplayMode DM;												// currently unused, gets maximum resolution of computer
	SDL_Texture* bg = NULL;											// background
	SDL_Texture* pause = NULL;
	SDL_Texture* revive = NULL;										// revive/dead background
	SDL_Surface* surface = NULL;									// surface for assigning textures
	SDL_Event e;													// a single keypress event

public:

	// self explanatory functions
	float getScreenHeight() { return this->SCREEN_HEIGHT; }
	float getScreenWidth() { return this->SCREEN_WIDTH; }

	void setScreenWidth(int width) { SCREEN_WIDTH = width; }
	void setScreenHeight(int height) { SCREEN_HEIGHT = height; }

	SDL_Texture *getBackground() { return this->bg; }
	SDL_Surface *getSurface() { return this->surface; }

	void init() { SDL_Init(SDL_INIT_VIDEO); SDL_Init(SDL_INIT_AUDIO); }
	void setRenderer(SDL_Window* window) { renderer.createRenderer(window); }

	void setBar()
	{
		healthBar.setObjectDest((getScreenWidth()*.015625), (getScreenHeight()*.9375), 
			(getScreenWidth()*.234375), (getScreenHeight()*.041666));
		healthBar.setObjectSrc(0, 0, 75, 10);
		surface = SDL_LoadBMP("images/health_bar_sheet.bmp");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
		healthBar.setImage(healthBar.createImage(surface,renderer));
	}

	void setBG()
	{
		surface = SDL_LoadBMP("images/Forest0.bmp");
		bg = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	}

	void setRevive()
	{
		surface = SDL_LoadBMP("images/revive.bmp");
		revive = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	}

	void setPause()
	{
		surface = SDL_LoadBMP("images/pause.bmp");
		pause = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
		SDL_SetTextureAlphaMod(pause, 200);
	}

	void checkWindowPos(Sprite &sprite) // "scene switching" (basically reverts position based on screen edge collision)
	{
		if (sprite.getdest().x > (getScreenWidth())+sprite.getdest().w - (getScreenWidth()*.03125))
			sprite.setDestX(-sprite.getdest().w+(getScreenWidth()*.03125));
		if (sprite.getdest().x < (-sprite.getdest().w + (getScreenWidth()*.03125)))
			sprite.setDestX((getScreenWidth()+sprite.getdest().w) - (getScreenWidth()*.03125));
	}

	void startPause(Sprite &sprite) // pause procedure, includes pause when and the various controls
	{
		paused = true; //the game is paused

		while (paused) //game stays paused until...
		{
			SDL_RenderClear(renderer.getRenderer());
			SDL_Delay(1000 / 30);

			renderer.renderBg(bg);
			renderer.renderHudObject(healthBar.getImage(), healthBar.getObjectSrc(), healthBar.getObjectDest());
			if (moving && !jumping)
				renderer.renderSprite(sprite.getSpriteMotionTexture(), sprite.isfacingright(), sprite.getsrc(), sprite.getdest());
			else if (jumping || (jumping && moving))
				renderer.renderSprite(sprite.getSpriteJumpTexture(), sprite.isfacingright(), sprite.getJumpSrc(), sprite.getdest());
			else if (!jumping && !moving)
				renderer.renderSprite(sprite.getSpriteStandTexture(), sprite.isfacingright(), sprite.getStandSrc(), sprite.getStandDest());
			renderer.renderBg(pause); // Alpha-modded background for fancy transparency
			SDL_RenderPresent(renderer.getRenderer());

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_KEYDOWN && e.type != SDL_KEYUP)
				{
					if (currentKeyStates[SDL_SCANCODE_C]) // continue
						paused = false;
					if (currentKeyStates[SDL_SCANCODE_Q]) // looks like we got a...
					{
						quitter = true; // QUITTER!!
						paused = false; // when there's a quitter, they forfeit their rights to game controls
					}	// endif-Q
				}		// endif-event
			}			// endwhile-event
		}				// endwhile-pause
	}

	void eventHandler(Sprite &sprite) // handles events, who would've thought
	{
		if (sprite.getHealth() <= 0) // you're dead
			dead = true;

		sprite.setLast();

		if ((currentKeyStates[SDL_SCANCODE_SPACE] || currentKeyStates[SDL_SCANCODE_UP]) && counter < 12)
		{
			counter += 1;
			sprite.beginJump(sprite, moving, getScreenHeight(), getScreenWidth());
			jumping = true; // you're jumpin'
		}
		else if (sprite.getdest().y < (getScreenHeight()*.58))
		{
			jumping = true; // you're jumpin' (well technically falling)
			sprite.drop(sprite, moving, getScreenHeight(), getScreenWidth());
			// stop, drop, but don't roll cause we haven't programmed it
			// play thud sound here?
			if (sprite.getdest().y > (getScreenHeight()*.58))
			{
				mixer.playThud();
			}
		}

		if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) // if you move right
		{
			if ((!mixer.isPlaying(1) || !mixer.isPlaying(2)) && !jumping)
				mixer.playStep();

			if (currentKeyStates[SDL_SCANCODE_LSHIFT]) // sprint == DOUBLE SPEED
			{
				if(currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
					sprite.move(sprite, mixer, SDL_SCANCODE_RIGHT, getScreenHeight(), getScreenWidth());

				else if(currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
					sprite.move(sprite, mixer, SDL_SCANCODE_LEFT, getScreenHeight(), getScreenWidth());

				if(!jumping)
					mixer.playStep();
			}

			if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
				sprite.move(sprite, mixer, SDL_SCANCODE_RIGHT, getScreenHeight(), getScreenWidth());

			else if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
				sprite.move(sprite, mixer, SDL_SCANCODE_LEFT, getScreenHeight(), getScreenWidth());

			moving = true; // you're moving
		}

		// debugging health system
		if (currentKeyStates[SDL_SCANCODE_H] && !dead) // A slow death
		{
			sprite.setHealth(sprite.getHealth() - 10);
			mixer.playHurt();
		}

		if (currentKeyStates[SDL_SCANCODE_G]) // a swift death
			sprite.setHealth(0);

		if (currentKeyStates[SDL_SCANCODE_R]) // resurrection
			sprite.setHealth(100);

		// pause
		if (currentKeyStates[SDL_SCANCODE_ESCAPE] && e.type == SDL_KEYDOWN && e.type != SDL_KEYUP)
			startPause(sprite);
	}

	void setup()		// creating window and renderer...
	{
		renderer.createWindow("Garbage Dungeon", getScreenWidth(), getScreenHeight());
		setRenderer(renderer.getWindow());
		setBG();
		setBar();
		setRevive();
		setPause();
		mixer.setSoundFiles();
	}

	void updateWin()	// if the window is rescaled
	{
		setScreenHeight(SDL_GetWindowSurface(renderer.getWindow())->h);
		setScreenWidth(SDL_GetWindowSurface(renderer.getWindow())->w);
		// do we call jump here?
	}

	void playDead()
	{
		mixer.playDeath();
		while (dead)	// stay dead until revived or there's a quitter, we don't want any zombies unless we say so
		{
			SDL_RenderClear(renderer.getRenderer());
			SDL_Delay(1000 / 30);

			renderer.renderBg(revive);
			renderer.renderHudObject(healthBar.getImage(), healthBar.getObjectSrc(), healthBar.getObjectDest());
			SDL_RenderPresent(renderer.getRenderer());

			while (SDL_PollEvent(&e) != 0)
			{

				carl.setDT();
				if (e.type == SDL_QUIT) // you're a quitter, stop lying to yourself
					endGame();

				else if (currentKeyStates[SDL_SCANCODE_R]) // ZOMBIE!!! but we said so, so it's okay... for now
				{
					dead = false;
					carl.setDestX(0);
					carl.setSrcX(0);
					carl.setSrcY(0);
					carl.setHealth(100);
				}

				if (currentKeyStates[SDL_SCANCODE_ESCAPE]) // yup you're still a quitter
					endGame();
			}
		}
	}

	void run()
	{
		// initializing...
		setup();

		carl.initCarlResources(renderer.getRenderer(), 0, (getScreenHeight()*.59), getScreenHeight(),getScreenWidth());

		evilCarl.initCarlResources(renderer.getRenderer(), 480, 
			(getScreenHeight()*.59), getScreenHeight(), getScreenWidth()); // added sprite, no controls

		carl.setHealth(100);

		while (!quitter)		// main game loop
		{
			//check for updated window heights and widths
			updateWin(); 
			carl.updateSprite(getScreenHeight(), getScreenWidth());
			evilCarl.updateSprite(getScreenHeight(), getScreenWidth());
			healthBar.setObjectDest((getScreenWidth()*.015625), (getScreenHeight()*.9375),
				(getScreenWidth()*.234375), (getScreenHeight()*.041666));

			if (dead)
				playDead();						// if you died...

			SDL_Delay(1000 / 30);
			while (SDL_PollEvent(&e) != 0)		// exit check
			{
				carl.setDT();
				if (e.type == SDL_QUIT)
					quitter = true;				// again, you're a quitter
			}

			checkWindowPos(carl);				// check sprite pos and simulate switching "scenes"
			eventHandler(carl);					// checking for key presses

			SDL_RenderClear(renderer.getRenderer());
			renderer.renderBg(bg);				// render background

			checkHealth(carl);					// check health and render bar to corresponding health
			renderer.renderHudObject(healthBar.getImage(), healthBar.getObjectSrc(), healthBar.getObjectDest());

			renderer.renderSprite(evilCarl.getSpriteStandTexture(), evilCarl.isfacingright(), 
				evilCarl.getStandSrc(), evilCarl.getStandDest()); // render evil carl

			if (moving && !jumping)				// if moving and not jumping, render running/walking animation
			{
				renderer.renderSprite(carl.getSpriteMotionTexture(), carl.isfacingright(), carl.getsrc(), carl.getdest());
				carl.setJumpSrcX(0);
				counter = 0;
			}

			if (jumping || (jumping && moving)) // if jumping OR jumping and moving, render jump animation
			{
				renderer.renderSprite(carl.getSpriteJumpTexture(), carl.isfacingright(), carl.getJumpSrc(), carl.getdest());
				if (carl.getJumpSrc().x == 0)
					carl.setJumpSrcX(121);
			}

			if (!moving && !jumping)			// if standing still, render standing animation --- should clean this up a bit
			{
				counter = 0;
				carl.setJumpSrcX(0);
				carl.setStandDestX(carl.getdest().x);
				carl.setStandDestY(carl.getdest().y);
				renderer.renderSprite(carl.getSpriteStandTexture(), carl.isfacingright(), carl.getStandSrc(), carl.getStandDest());
				if (carl.getStandSrc().x < 2052)
					carl.setStandSrcX(carl.getStandSrc().x + 92);
				if (carl.getStandSrc().x >= 2052)
					carl.setStandSrcX(0);
				renderer.renderSprite(carl.getSpriteStandTexture(), carl.isfacingright(), carl.getStandSrc(), carl.getStandDest());
			}

			moving = false;						// reset jumping and moving for re-evaluation
			jumping = false;

			SDL_RenderPresent(renderer.getRenderer());
		}

		// clean up after ourselves when there's a quitter
		endGame();
	}

	void checkHealth(Sprite sprite)
	{
		if(sprite.getHealth() < 0)		// after becoming a zombie, set health to full
			sprite.setHealth(100);

		int hp = sprite.getHealth();	// variable for switch

		switch (hp)						// update health bar based on current health
		{
		case 100:
			healthBar.setObjectSrcX(0);
			healthBar.setObjectSrcY(0);
			break;
		case 90:
			healthBar.setObjectSrcX(0);
			healthBar.setObjectSrcY(11);
			break;
		case 80:
			healthBar.setObjectSrcX(0);
			healthBar.setObjectSrcY(22);
			break;
		case 70:
			healthBar.setObjectSrcX(0);
			healthBar.setObjectSrcY(33);
			break;
		case 60:
			healthBar.setObjectSrcX(0);
			healthBar.setObjectSrcY(44);
			break;
		case 50:
			healthBar.setObjectSrcX(80);
			healthBar.setObjectSrcY(0);
			break;
		case 40:
			healthBar.setObjectSrcX(80);
			healthBar.setObjectSrcY(11);
			break;
		case 30:
			healthBar.setObjectSrcX(80);
			healthBar.setObjectSrcY(22);
			break;
		case 20:
			healthBar.setObjectSrcX(80);
			healthBar.setObjectSrcY(33);
			break;
		case 10:
			healthBar.setObjectSrcX(80);
			healthBar.setObjectSrcY(44);
			break;
		case 0:
			dead = true;
			healthBar.setObjectSrcX(80);
			healthBar.setObjectSrcY(55);
			break;
		}
	}

	void endGame() // destroy all the things!
	{
		SDL_DestroyTexture(bg);
		SDL_DestroyTexture(revive);
		SDL_DestroyTexture(pause);
		healthBar.destroyImage();
		SDL_FreeSurface(surface);
		mixer.killMusic();
		SDL_DestroyRenderer(renderer.getRenderer());
		SDL_DestroyWindow(renderer.getWindow());
		
		SDL_Delay(100);
		SDL_Quit();
	}
};