#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
#define TOKAY_PATH		"tokay.bmp"
SDL_Window* window = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* tokay = NULL;

bool init() 
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Cannot init SDL: %s\n", SDL_GetError());
		return false;
	}
	window = SDL_CreateWindow(
			"Surfaces", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window) {
		fprintf(stderr, "Cannot init window: %s\n", SDL_GetError());
		return false;
	}
	screen = SDL_GetWindowSurface(window);
	int bgcolor = SDL_MapRGB(screen->format, 0x80, 0x80, 0xaa);
	SDL_FillRect(screen, NULL, bgcolor);
	SDL_UpdateWindowSurface(window);
	return true;
}

void quit() 
{
	if(tokay)
		SDL_FreeSurface(tokay);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


SDL_Surface* loadSurface(char *path) 
{
	SDL_Surface* loadedSurface;
	SDL_Surface* optimizedSurface;
	// load initial surface //	
	loadedSurface = SDL_LoadBMP(path);

	if(!loadedSurface) {
		fprintf(stderr, "Cannot load %s: %s\n", path, SDL_GetError());
		return NULL;
	}
	// convert to optimized, screen format type //
	optimizedSurface = SDL_ConvertSurface(loadedSurface, screen->format, 0);
	if(!optimizedSurface) {
		fprintf(stderr, "Cannot optimize surface: %s\n", SDL_GetError());
		return NULL;
	}
	// free initial surface //
	SDL_FreeSurface(loadedSurface);
	return optimizedSurface;
}

int main() 
{
	if(!init()) 
		return 1;
	if((tokay = loadSurface(TOKAY_PATH)) == NULL)
		return 1;

	SDL_Rect strechRect;
	strechRect.x = 0;
	strechRect.y = 0;
	strechRect.w = SCREEN_WIDTH;
	strechRect.h = SCREEN_HEIGHT;

	//SDL_BlitSurface(tokay, NULL, screen, NULL);
	SDL_BlitScaled(tokay, NULL, screen, &strechRect);
	SDL_UpdateWindowSurface(window);

	SDL_Delay(2000);
	
	quit();
}
