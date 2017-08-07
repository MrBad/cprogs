#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TOKAY_PATH "tokay.bmp"

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* tokay = NULL;
bool quitGame = false;
SDL_Event e;

bool init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow(
			"SDL events", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window) {
		fprintf(stderr, "Cannot create window: %s\n", SDL_GetError());
		return false;
	}

	screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x60, 0x60, 0x80));
	SDL_UpdateWindowSurface(window);
	return true;
}

void quit() 
{
	if(tokay != NULL)
		SDL_FreeSurface(tokay);
	if(window != NULL)
		SDL_DestroyWindow(window);
	SDL_Quit();
}

bool loadMedia() 
{
	tokay = SDL_LoadBMP(TOKAY_PATH);
	if(!tokay) {
		fprintf(stderr, "Cannot load tokay: %s\n", SDL_GetError());
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

	while(!quitGame) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quitGame = true;
			} 
			else if(e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym) {
					case SDLK_UP:
						printf("up\n"); break;
					case SDLK_DOWN:
						printf("down\n"); break;
					case SDLK_LEFT:
						printf("left\n"); break;
					case SDLK_RIGHT:
						printf("right\n"); break;
					default:
						break;
				}
			}
			else {
				fprintf(stdout, "event: %d\n", e.type);
			}
		}
	}

	quit();
	return 0;
}
