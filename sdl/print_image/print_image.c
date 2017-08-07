#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window;
SDL_Surface* screen;
SDL_Surface* tokay;

bool init() 
{
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stdout, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	window = SDL_CreateWindow(
			"SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if(window == NULL) {
		return false;
	}
	screen = SDL_GetWindowSurface(window);

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x60, 0x60, 0x80));
	SDL_UpdateWindowSurface(window);
	return true;
}

void quit()
{
	SDL_FreeSurface(tokay);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool loadMedia() 
{
	tokay = SDL_LoadBMP("tokay.bmp");
	if(tokay == NULL) {
		fprintf(stdout, "Cannot load tokay.bmp: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

int main()
{
	if(!init()) {
		return 1;
	}
	if(!loadMedia()) { 
		return 1;
	}
	SDL_BlitSurface(tokay, NULL, screen, NULL);
	SDL_UpdateWindowSurface(window);
	SDL_Delay(2000);
	
	quit();
	
	return 0;
}
