#include <iostream>
#include <SDL.h>

#include "Game.h"
#include "Tool.h"

Game test(20, 10);

void update() {
	Tool::updateKeyboardState();

	if (Tool::getKeyboardState(SDL_SCANCODE_W)) {
		test.setDirection(Direction::up);
	}
	else if (Tool::getKeyboardState(SDL_SCANCODE_S)) {
		test.setDirection(Direction::down);
	}
	else if (Tool::getKeyboardState(SDL_SCANCODE_D)) {
		test.setDirection(Direction::right);
	}
	else if (Tool::getKeyboardState(SDL_SCANCODE_A)) {
		test.setDirection(Direction::left);
	}
	else {
		return;
	}
	test.update();
	system("cls");
	
	std::vector<float> temp = test.getData();
	for (int at = 0; at < temp.size(); at++) {
		if (at % 8 == 0) {
			std::cout << "\n";
		}
		std::cout << temp.at(at) << std::endl;
	}
}

void render(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	test.render(renderer, 50, 50, 400);

	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Neural Network", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;
	bool running = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(renderer, 700, 700);

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		update();
		render(renderer);
		SDL_Delay(100);
	}

	SDL_Quit();
	return 0;
}