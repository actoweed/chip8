#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdio>

int main() {
	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_VIDEO !=0)) {
		std::printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Window* window = SDL_CreateWindow(
		"CHIP-8",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640,320,
		0);
	SDL_Renderer* renderer = 
		SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool running = true;
	while (running){
		
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
		
		SDL_SetRenderDrawColor(renderer,30,30,46,255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
