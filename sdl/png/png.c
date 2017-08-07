#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* pngSurface = NULL;


bool init()
{
	if(SDL_Init( SDL_INIT_VIDEO ) < 0) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return false;
	}
	
	window = SDL_CreateWindow(
			"Load a png", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window) {
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		return false;
	}

	// initializa image //
	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags) & imgFlags)) {
		fprintf(stderr, "SDL_image could not init: %s\n", IMG_GetError());
		return false;
	}
	screen = SDL_GetWindowSurface(window);
	unsigned int bgcolor = SDL_MapRGB(screen->format, 0x60, 0x60, 0x80);
	SDL_FillRect(screen, NULL, bgcolor);
	SDL_UpdateWindowSurface(window);
	return true;
}

void quit()
{

	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface(char *path) 
{
	SDL_Surface* loadedSurface;
	SDL_Surface* optimizedSurface;

	loadedSurface = IMG_Load(path);
	if(!loadedSurface) {
		fprintf(stderr, "Cannot load surface: %s, %s\n", path, IMG_GetError());
		return NULL;
	}
	optimizedSurface = SDL_ConvertSurface(loadedSurface, screen->format, 0);
	if(!optimizedSurface) {
		fprintf(stderr, "Cannot optimize surface: %s\n", SDL_GetError());
		return NULL;
	}
	SDL_FreeSurface(loadedSurface);
	return optimizedSurface;
}

bool loadMedia() 
{
	pngSurface = loadSurface("wallpaper.png");
	if(pngSurface == NULL) {
		fprintf(stderr, "Cannot load wallpaper\n");
		return false;
	}
	return true;
}

int main()
{

	bool quitGame = false;
	SDL_Event e;

	if(!init()) 
		return 1;
	if(!loadMedia()) 
		return 1;

	while(!quitGame) {
		// handle events on queue //
		while(SDL_PollEvent(&e) != 0) {
			// if user click X
			if(e.type == SDL_QUIT) {
				quitGame = true;
			} 
			// if use press ESC
			else if(e.type == SDL_KEYDOWN) {
				if(e.key.keysym.sym == SDLK_ESCAPE) {
					quitGame = true;
				}
			}
		}

		SDL_BlitSurface(pngSurface, NULL, screen, NULL);
		SDL_UpdateWindowSurface(window);
	}
	
	
	quit();
	return 0;
}
