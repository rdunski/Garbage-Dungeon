#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <string>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Sprite constants
const int SPRITE_WIDTH = (SCREEN_WIDTH*.1);
const int SPRITE_HEIGHT = (SCREEN_HEIGHT*.1);

//Motion constants
const float X_PERCENTAGE = .004; //left and right
const float Y_NEG_PERCENTAGE = .007; //jumping
const float Y_POS_PERCENTAGE = .02; //falling

//float pX = 0;
//float aX = 50;
//float aY = 100;
//float pY = SCREEN_HEIGHT - 100;
//float posX = 0;
//float vX = 0;
//float vY = 0;

//Keyboard input handling
const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Loads individual image as texture
SDL_Texture* loadTexture(string path);

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* background = NULL;

SDL_Texture* ground = NULL;

SDL_Texture* Jim1 = NULL;
SDL_Texture* Jim2 = NULL;
SDL_Texture* Jim3 = NULL;
SDL_Texture* Jim4 = NULL;

SDL_RendererFlip spriteFlip = SDL_FLIP_HORIZONTAL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

SDL_Texture* loadTexture(string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	background = loadTexture("forest.png");
	if (background == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	Jim1 = loadTexture("sprite_0.png");
	if (Jim1 == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	Jim2 = loadTexture("sprite_1.png");
	if (Jim2 == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	Jim3 = loadTexture("sprite_2.png");
	if (Jim3 == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	Jim4 = loadTexture("sprite_3.png");
	if (Jim4 == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	ground = loadTexture("bricks.png");
	if (ground == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture(background);
	background = NULL;

	//Destroy window    
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

//void motionX(float dt)
//{
//	vX = vX + aX*dt;
//	pX = pX + vX*dt;
//}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		bool quit = false;
		bool forwardLastPress = true;

		//counter for frames
		int x = 0;

		SDL_Event e;

		//Viewport Rectangles
		SDL_Rect topRect;
		SDL_Rect bottomRect;
		SDL_Rect spriteRect;

		spriteRect.x = 0;
		spriteRect.y = SCREEN_HEIGHT - ((SCREEN_HEIGHT*.10)+SPRITE_HEIGHT);
		spriteRect.w = SPRITE_WIDTH;
		spriteRect.h = SPRITE_HEIGHT;

		topRect.x = 0;
		topRect.y = 0;
		topRect.w = SCREEN_WIDTH;
		topRect.h = SCREEN_HEIGHT - (SCREEN_HEIGHT*.10);

		bottomRect.x = 0;
		bottomRect.y = SCREEN_HEIGHT - (SCREEN_HEIGHT*.10);
		bottomRect.w = SCREEN_WIDTH;
		bottomRect.h = SCREEN_HEIGHT - (SCREEN_HEIGHT*.90);

		/*
		-----Stuff for realistic motion (like we ever need that)----

		float last = SDL_GetTicks();
		float vY = vY + aY*dt
		float pY = pY+vY*dt
		float vX = vX +aX*dt
		float pX = pX +vX*dt
		float dt = ((float)SDL_GetTicks() - last) / 1000;
		*/

		if (!loadMedia()) //initialize frames, background images, foreground images, etc.
		{
			printf("Failed to load media!\n");
		}
		else
		{
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_RenderClear(gRenderer);

				SDL_RenderSetViewport(gRenderer, &topRect);

				SDL_RenderCopy(gRenderer, background, NULL, NULL);

				SDL_RenderSetViewport(gRenderer, &spriteRect);

				SDL_Delay(1000 / 60);

				if (!currentKeyStates[SDL_SCANCODE_D] && forwardLastPress)
					SDL_RenderCopy(gRenderer, Jim1, NULL, NULL);
				if(!currentKeyStates[SDL_SCANCODE_A] && !forwardLastPress)
					SDL_RenderCopyEx(gRenderer, Jim1, NULL, NULL, NULL, NULL, spriteFlip);

				//The jump
				if (currentKeyStates[SDL_SCANCODE_SPACE]
					&& spriteRect.y == SCREEN_HEIGHT - ((SCREEN_HEIGHT*.10) + SPRITE_HEIGHT))
				{
					//float dt = ((float)SDL_GetTicks() - last) / 1000;
					//last = SDL_GetTicks();
					if (spriteRect.y >= 0 && spriteRect.y <= (SCREEN_HEIGHT - (SCREEN_HEIGHT*.10)))
					{
						if (currentKeyStates[SDL_SCANCODE_D])
						{
							forwardLastPress = true;
							spriteRect.x += SCREEN_WIDTH*X_PERCENTAGE;
						}
							//motionX(dt);
						if (currentKeyStates[SDL_SCANCODE_A])
						{
							forwardLastPress = false;
							spriteRect.x -= SCREEN_WIDTH*X_PERCENTAGE;
						}
							//vX = vX + aX*dt;
							//pX = pX - vX*dt;
						//vY = vY + aY*dt;
						//pY = pY - vY*dt;
						//spriteRect.y = (int)pY;
						for (int x = 0; x < 30; x++) //plays a part in the max height of the jump
						{
							spriteRect.y -= SCREEN_HEIGHT*Y_NEG_PERCENTAGE;
							SDL_RenderSetViewport(gRenderer, &spriteRect);

							if((x == 5 || x == 10 || x == 20 || x == 29) && forwardLastPress)
								SDL_RenderCopy(gRenderer, Jim1, NULL, NULL);
							if ((x == 5 || x == 10 || x == 20 || x == 29) && !forwardLastPress)
								SDL_RenderCopyEx(gRenderer, Jim1, NULL, NULL, NULL, NULL, spriteFlip);
						}
					}
				}

				//check to see if sprite is out of the window
				if (spriteRect.x > SCREEN_WIDTH-SPRITE_WIDTH)
					spriteRect.x = SCREEN_WIDTH - SPRITE_WIDTH;
				if (spriteRect.x < 0)
					spriteRect.x = 0;
				//check to see if sprite is falling through the floor
				if (spriteRect.y > (SCREEN_HEIGHT - ((SCREEN_HEIGHT*.10) + SPRITE_HEIGHT)))
					spriteRect.y = SCREEN_HEIGHT - ((SCREEN_HEIGHT*.10) + SPRITE_HEIGHT);

				//check to see if sprite is in the air without jumping and make it fall
				if (spriteRect.y < (SCREEN_HEIGHT - ((SCREEN_HEIGHT*.10) + SPRITE_HEIGHT)))
				{
					//float dt = ((float)SDL_GetTicks() - last) / 1000;
					//last = SDL_GetTicks();
					if (currentKeyStates[SDL_SCANCODE_D])
						spriteRect.x += SCREEN_WIDTH*X_PERCENTAGE;
						//motionX(dt);
					if (currentKeyStates[SDL_SCANCODE_A])
						spriteRect.x -= SCREEN_WIDTH*X_PERCENTAGE;
						//vX = vX + aX*dt;
						//pX = pX - vX*dt;
					//vY = vY + aY*dt;
					//pY = pY + vY*dt;
					spriteRect.y += SCREEN_HEIGHT*Y_POS_PERCENTAGE;
					SDL_RenderSetViewport(gRenderer, &spriteRect);

					SDL_RenderCopy(gRenderer, Jim1, NULL, NULL);
				}
				if (spriteRect.y > (SCREEN_HEIGHT - ((SCREEN_HEIGHT*.10) + SPRITE_HEIGHT)))
					spriteRect.y = SCREEN_HEIGHT - ((SCREEN_HEIGHT*.10) + SPRITE_HEIGHT);

				//Moving Right
				if (currentKeyStates[SDL_SCANCODE_D] && spriteRect.x != (SCREEN_WIDTH-SPRITE_WIDTH))
				{
					forwardLastPress = true;
					/*if (vX < 0)
						vX = 0;
						motionX(dt);*/
					spriteRect.x += SCREEN_WIDTH*X_PERCENTAGE;

					SDL_RenderSetViewport(gRenderer, &spriteRect);

					if (x >= 0 && x < 15) //Animate! x is a basic counter
					{
						SDL_RenderCopy(gRenderer, Jim4, NULL, NULL);
						x++;
					}
					else if (x >= 15 && x <30)
					{
						SDL_RenderCopy(gRenderer, Jim1, NULL, NULL);
						x++;
					}
					else if (x >= 30 && x < 45)
					{
						SDL_RenderCopy(gRenderer, Jim2, NULL, NULL);
						x++;
					}
					else if (x >= 45 && x < 60)
					{
						SDL_RenderCopy(gRenderer, Jim1, NULL, NULL);
						x++;
					}
					else if (x == 60)
						x = 0; //Reset counter
				}

				//Moving Left
				if (currentKeyStates[SDL_SCANCODE_A] && spriteRect.x != 1)
				{
					forwardLastPress = false;
					/*float dt = ((float)SDL_GetTicks() - last) / 1000;
					last = SDL_GetTicks();
					if (vX > 0)
						vX = 0;
					vX = -1*(vX + aX*dt);
					pX = (pX + vX*dt);*/
					spriteRect.x -= SCREEN_WIDTH*X_PERCENTAGE;
					SDL_RenderSetViewport(gRenderer, &spriteRect);

					if (x >= 0 && x < 15) //Animate! x is a basic counter
					{
						SDL_RenderCopyEx(gRenderer, Jim4, NULL, NULL, NULL, NULL, spriteFlip);
						x++;
					}
					else if (x >= 15 && x <30)
					{
						SDL_RenderCopyEx(gRenderer, Jim1, NULL, NULL, NULL, NULL, spriteFlip);
						x++;
					}
					else if (x >= 30 && x < 45)
					{
						SDL_RenderCopyEx(gRenderer, Jim2, NULL, NULL, NULL, NULL, spriteFlip);
						x++;
					}
					else if (x >= 45 && x < 60)
					{
						SDL_RenderCopyEx(gRenderer, Jim1, NULL, NULL,NULL,NULL,spriteFlip);
						x++;
					}
					else if (x == 60)
						x = 0; //Reset counter
				}

				//The floor
				SDL_RenderSetViewport(gRenderer, &bottomRect);
				SDL_RenderCopy(gRenderer, ground, NULL, NULL);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	//Free resources and close SDL
	close();

	return 0;
}