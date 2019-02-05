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
#include "UI.h"
using namespace std;

class Game {
protected:
	Render renderer;												// the renderer...
	UI face;														// the user interface
	Sprite carl, evilCarl;											// our guy (also evil Carl)
	Sound mixer;													// da sound machine
	bool quitter, dead, moving, jumping, paused = false;			// boolean checks
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);		// keyboard press events
	int counter = 0;												// counter for jumping
	int start = 0;
	SDL_DisplayMode DM;												// currently unused, gets maximum resolution of computer
	SDL_Event e;													// a single keypress event

public:

	// self explanatory functions

	void init() { SDL_Init(SDL_INIT_VIDEO); SDL_Init(SDL_INIT_AUDIO); }
	void setRenderer(SDL_Window* window) { renderer.createRenderer(window); }

	void startPause(Sprite &sprite) // pause procedure, includes pause and the various controls
	{
		paused = true; //the game is paused

		while (paused) //game stays paused until...
		{
			SDL_RenderClear(renderer.getRenderer());
			SDL_Delay(1000 / 30);

			renderer.renderBg(face.getBackground());
			renderer.renderHudObject(face.getBar().getImage(), face.getBar().getObjectSrc(), face.getBar().getObjectDest());
			if (moving && !jumping)
				renderer.renderSprite(sprite.getSpriteMotionTexture(), sprite.isfacingright(), sprite.getsrc(), sprite.getdest());
			else if (jumping || (jumping && moving))
				renderer.renderSprite(sprite.getSpriteJumpTexture(), sprite.isfacingright(), sprite.getJumpSrc(), sprite.getdest());
			else if (!jumping && !moving)
				renderer.renderSprite(sprite.getSpriteStandTexture(), sprite.isfacingright(), sprite.getStandSrc(), sprite.getStandDest());
			renderer.renderBg(face.getPause()); // Alpha-modded background for fancy transparency
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
			sprite.beginJump(sprite, moving, face.getScreenHeight(), face.getScreenWidth());
			jumping = true; // you're jumpin'
		}
		else if (sprite.getdest().y < (face.getScreenHeight()*.58))
		{
			jumping = true; // you're jumpin' (well technically falling)
			sprite.drop(sprite, moving, face.getScreenHeight(), face.getScreenWidth());
			// stop, drop, but don't roll cause we haven't programmed it
			// play thud sound here?
			if (sprite.getdest().y > (face.getScreenHeight()*.58))
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
					sprite.move(sprite, mixer, SDL_SCANCODE_RIGHT, face.getScreenHeight(), face.getScreenWidth());

				else if(currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
					sprite.move(sprite, mixer, SDL_SCANCODE_LEFT, face.getScreenHeight(), face.getScreenWidth());

				if(!jumping)
					mixer.playStep();
			}

			if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
				sprite.move(sprite, mixer, SDL_SCANCODE_RIGHT, face.getScreenHeight(), face.getScreenWidth());

			else if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
				sprite.move(sprite, mixer, SDL_SCANCODE_LEFT, face.getScreenHeight(), face.getScreenWidth());

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
		renderer.createWindow("Garbage Dungeon", face.getScreenWidth(), face.getScreenHeight());
		setRenderer(renderer.getWindow());
		face.setBG(renderer);
		face.setBar(renderer);
		face.setRevive(renderer);
		face.setPause(renderer);
		mixer.setSoundFiles();
	}

	void playDead()
	{
		mixer.playDeath();
		while (dead)	// stay dead until revived or there's a quitter, we don't want any zombies unless we say so
		{
			SDL_RenderClear(renderer.getRenderer());
			SDL_Delay(1000 / 30);

			renderer.renderBg(face.getRevive());
			renderer.renderHudObject(face.getBar().getImage(), face.getBar().getObjectSrc(), face.getBar().getObjectDest());
			SDL_RenderPresent(renderer.getRenderer());

			while (SDL_PollEvent(&e) != 0)
			{

				carl.setDT();
				if (e.type == SDL_QUIT) // you're a quitter, stop lying to yourself
					endGame();

				else if (currentKeyStates[SDL_SCANCODE_R]) // ZOMBIE!!! but we said so, so it's okay... for now
				{
					dead = false;
					evilCarl.setDestX(480);
					evilCarl.setSrcX(0);
					evilCarl.setSrcY(0);
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

		carl.initCarlResources(renderer.getRenderer(), 0, (face.getScreenHeight()*.59), face.getScreenHeight(),
			face.getScreenWidth());

		evilCarl.initCarlResources(renderer.getRenderer(), 480, 
			(face.getScreenHeight()*.59), face.getScreenHeight(), face.getScreenWidth()); // added sprite, no controls

		carl.setHealth(100);

		while (!quitter)		// main game loop
		{
			//check for updated window heights and widths
			face.updateWin(renderer); 
			carl.updateSprite(face.getScreenHeight(), face.getScreenWidth());
			evilCarl.updateSprite(face.getScreenHeight(), face.getScreenWidth());
			face.getBar().setObjectDest((face.getScreenWidth()*.015625), (face.getScreenHeight()*.9375),
				(face.getScreenWidth()*.234375), (face.getScreenHeight()*.041666));

			if (dead)
				playDead();						// if you died...

			SDL_Delay(1000 / 30);
			while (SDL_PollEvent(&e) != 0)		// exit check
			{
				carl.setDT();
				if (e.type == SDL_QUIT)
					quitter = true;				// again, you're a quitter
			}

			face.checkWindowPos(carl);				// check sprite pos and simulate switching "scenes"
			eventHandler(carl);					// checking for key presses

			SDL_RenderClear(renderer.getRenderer());
			renderer.renderBg(face.getBackground());				// render background

			face.checkHealth(carl);					// check health and render bar to corresponding health
			renderer.renderHudObject(face.getBar().getImage(), face.getBar().getObjectSrc(), 
				face.getBar().getObjectDest());

			face.checkWindowPos(evilCarl);
			evilCarl.move(evilCarl, mixer, SDL_SCANCODE_LEFT, face.getScreenHeight(), face.getScreenWidth());
			renderer.renderSprite(evilCarl.getSpriteMotionTexture(), evilCarl.isfacingright(), 
				evilCarl.getsrc(), evilCarl.getdest()); // render evil carl running left

			//if (SDL_HasIntersection(&evilCarl.getdest(), &carl.getdest()) && !dead) // if you touch evil carl you get hurted
			//{
			//	//carl.setHealth(carl.getHealth() - 10);
			//	if ((SDL_GetTicks() - start) > 1000) {
			//		carl.setHealth(carl.getHealth() - 1);				// HE NEEDS SOME MILK
			//		mixer.playHurt();
			//	}
			//}
			//start = SDL_GetTicks();

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

	void endGame() // destroy all the things!
	{
		SDL_DestroyTexture(face.getBackground());
		SDL_DestroyTexture(face.getRevive());
		SDL_DestroyTexture(face.getPause());
		face.getBar().destroyImage();
		SDL_FreeSurface(face.getSurface());
		mixer.killMusic();
		SDL_DestroyRenderer(renderer.getRenderer());
		SDL_DestroyWindow(renderer.getWindow());
		
		SDL_Delay(100);
		SDL_Quit();
	}
};