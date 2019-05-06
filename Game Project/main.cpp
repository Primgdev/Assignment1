#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>//text to font

using namespace std;


int main(int argc, char** argv){
	//initialise SDL with all its sub systems e.g video, input, sound etc
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cout << "SDL failed: " << SDL_GetError() << endl;
		system("pause");
		return -1;
	}

	//Need to initialise other sub libraries
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		cout << "sdl image failed: " << IMG_GetError() << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//INIT sdl ttf
	if (TTF_Init() != 0){
		cout << "sdl ttf failed: " << TTF_GetError() << endl;
		system("pause");
		SDL_Quit();
		return -1;
	}

	//create window to render into
	//params: (title, position x, position y, width, height, window flags)
	SDL_Window* window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_SHOWN); //fullscreen with last param like this: SDL_WINDOW_SHOWN|SDL_WINDOW_FULLSCREEN

	if (window == NULL)
	{
		cout << "window failed: " << SDL_GetError() << endl;
		system("pause");
		SDL_Quit(); //cleans up SDL for us
		return -1;
	}

	//create a renderer, which helps draw stuff to the window
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL){
		cout << "renderer failed: " << SDL_GetError() << endl;
		system("pause");
		SDL_Quit(); //cleans up SDL for us
		return -1;
	}

	//load image into ram, as a surface we can access and tweak pixel data if we like
	SDL_Surface* knightSurface = SDL_LoadBMP("assets/knight.bmp");
	//convert surface into texture. Texture will be stored in VRAM for our graphics card to use
	SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, knightSurface);
	//dont need surface anymore, lets free its memory
	SDL_FreeSurface(knightSurface);

	
	SDL_Rect sourceRect;
	sourceRect.x = 50;
	sourceRect.y = 60;

	
	sourceRect.w = 80;
	sourceRect.h = 80;


	//where to draw our knight
	SDL_Rect destinationRect;
	destinationRect.x = 100;
	destinationRect.y = 100;
	destinationRect.w = sourceRect.w * 4;
	destinationRect.h = sourceRect.h * 2;

	//run texture
	SDL_Texture* runSpriteSheet = IMG_LoadTexture(renderer, "assets/run.png");
	//lets just draw 1 frame of this spritesheet for now
	SDL_Rect runClipRect = { 0, 0, 32, 32 }; //x=0, y=0, w=32,h=32
	SDL_Rect runDestRect = { 0, 0, 128, 128 };
	//lets make a version of run where background purple is transparent
	SDL_Surface* runSurface = IMG_Load("assets/run.png");
	//set colour key
	//params: surface
	//		  1 = turn on colour key, 0 is off
	//        colour to act as trasparent colour. SDL_MapRGB matches r g b values to the closest match it has in its palette
	SDL_SetColorKey(runSurface, 1, SDL_MapRGB(runSurface->format, 128, 128, 255));
	//convert to texture
	SDL_Texture* runSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, runSurface);

	SDL_FreeSurface(runSurface);




	//SETUP SOME TEXT
	TTF_Font* font = TTF_OpenFont("assets/vermin_vibes_1989.ttf", 60);//font file and font size
	SDL_Color textColour = { 123, 0, 34, 0 };
	//create surface using font, colour and some sort of message
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, " WELCOME TO THE GAME ", textColour);
	
	//convert surface to texture
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	

	//text destination
	SDL_Rect textDestination;
	textDestination.x = 100;
	textDestination.y = 100;
	//query to get width and height
	SDL_QueryTexture(textTexture, NULL, NULL, &textDestination.w, &textDestination.h);
	
	

	//stuff for time management
	Uint32 lastUpdate = SDL_GetTicks();

	bool loop = true;
	while (loop){
		//time management, work out Delta Time
		//=====================================
		//how many milliseconds since the last time this part of the loop ran
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		//convert whole milliseconds into a fraction of a second for DT
		float DT = timeDiff / 1000.0; //e.g 200ms = 0.2 for DT
		//update last update so next loop calculates properly
		lastUpdate = SDL_GetTicks();


		//Poll inputs
		SDL_Event event;
		//loop through all generated inputs
		while (SDL_PollEvent(&event)){
			//check if window closed
			if (event.type == SDL_QUIT){
				loop = false;
			}
			//check if they pressed the ESC key
			if (event.type == SDL_KEYDOWN){
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
					loop = false;
				}
			}




			//render text last ontop of all the other things
			SDL_RenderCopy(renderer, textTexture, NULL, &textDestination);


			//render to the screen
			SDL_RenderPresent(renderer);

			system("pause");

		}
		
		//CLEAN UP
		SDL_DestroyTexture(textTexture);
		SDL_DestroyTexture(knightTexture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		system("pause");
		return 0;
	}
}