#include "chip8.h"
#include <SDL_events.h>
#include <SDL_keycode.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdio>

int main (){
	SDL_SetMainReady();
	Chip8 chip8;
	    if (!chip8.loadRom("roms/2-ibm-logo.ch8")) {
        std::printf("Failed to load ROM\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        std::printf("%04X\n", chip8.fetch());
    }
	if (SDL_Init(SDL_INIT_VIDEO != 0)) {
		std::printf("SDL Init error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Window* window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640,320, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT ||(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
				running = false;
			} 
		}
		SDL_SetRenderDrawColor(renderer, 120,20,40,255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		

	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
