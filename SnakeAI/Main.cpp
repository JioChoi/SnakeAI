#include <iostream>
#include <fstream>
#include <thread>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "Ai.h"
#include "Tool.h"
#include "Font.h"

#include "Settings.h"

std::string fileName = "";
int mutationRate = 10;
int runTill = 50;
int oneGeneration = 500;
int mode = 0;

int runCount = 0;

struct Individual {
	std::shared_ptr<Ai> ai;
	std::shared_ptr<Game> game;
};

struct scoredIndividual {
	std::shared_ptr<Ai> ai;
	double score;
	std::vector<SDL_Point> apple;
	int length;
};

std::vector<Individual> ai;
std::vector<scoredIndividual> scoredAi;
std::vector<scoredIndividual> parent;
int generation = 1;
int individual = 0;
int processMode = 0;

double bestScore = 0;
double avgLength = 0;
double lengthSum = 0;
int bestLength = 0;
int generationBestLength = 0;

int seed = 10;

int deadNum = 0;
int createNum = 0;

void updateIndividual(int aiAt) {
	createNum++;
	while (true) {
		ai.at(aiAt).game.get()->getData(ai.at(aiAt).ai.get()->input);
		int result = ai.at(aiAt).ai.get()->calculate();
		ai.at(aiAt).game.get()->moveDirection(result - 1);
		ai.at(aiAt).game.get()->update();
		if (ai.at(aiAt).game.get()->dead == true) {
			ai.at(aiAt).game.get()->threadEnded = true;
			
			return;
		}

		if (processMode == 0) {
			SDL_Delay(50);
		}
		else if (processMode == 3) {
			SDL_Delay(20);
		}
	}
}

void putIndividual(int at) {
	std::thread t(updateIndividual, at);
	t.detach();
}

void reset() {
	Tool::resetSeed();
	fileName = "m" + std::to_string(mode) + "_mu" + std::to_string(mutationRate) + "_indi" + std::to_string(oneGeneration) + "_maxGen" + std::to_string(runTill);

	individual = 0;
	generation = 1;
	bestScore = 0;
	avgLength = 0;
	generationBestLength = 0;

	ai.clear();
	scoredAi.clear();
	parent.clear();

	std::ofstream output("result/" + fileName + ".res");
	output.clear();
	output.close();

	std::ofstream csv("result/" + fileName + ".csv");
	output.clear();
	output.close();
}

bool compareFunction(const scoredIndividual &a, const scoredIndividual &b) {
	return a.score > b.score;
}

void keyboardInput() {
	if (Tool::getKeyboardState(SDL_SCANCODE_1)) {
		processMode = 0;
	}
	else if (Tool::getKeyboardState(SDL_SCANCODE_2)) {
		processMode = 3;
	}
	else if (Tool::getKeyboardState(SDL_SCANCODE_3)) {
		processMode = 1;
	}
	else if (Tool::getKeyboardState(SDL_SCANCODE_4)) {
		processMode = 2;
	}
}

void update() {
	Tool::updateKeyboardState();

	keyboardInput();

	/* UPDATE AI & GAME */
	bool allDead = true;
	for (int at = 0; at < ai.size(); at++) {
		if (ai.at(at).game.get()->threadEnded == false) {
			allDead = false;
		}
	}

	/* CREATE NEW INDIVIDUAL / GENERATION */
	if (allDead) {
		for (int at = 0; at < CALCULATE_ONCE; at++) {
			int liveTime = ai.at(at).game.get()->liveTime;
			int eatenApple = ai.at(at).game.get()->eatenApple;
			double closePoint = ai.at(at).game.get()->closePoint;
			double score = 0;

			switch (mode) {
			case 0:
				score = eatenApple;
				break;
			case 1:
				score = liveTime + closePoint * 5;
				break;
			case 2:
				score = liveTime;
				break;
			case 3:
				score = eatenApple + closePoint;
				break;
			}

			scoredAi.push_back({ ai.at(at).ai, score, ai.at(at).game.get()->applePositionLog, ai.at(at).game.get()->length });
			bestScore = std::fmax(score, bestScore);
			lengthSum += ai.at(at).game.get()->length;
			bestLength = std::max(bestLength, ai.at(at).game.get()->length);
			generationBestLength = std::max(generationBestLength, ai.at(at).game.get()->length);
		}
		ai.clear();

		if (individual >= oneGeneration) {
			/* TO PREVENT DEPROVING */
			/*if (generation != 1) {
				scoredAi.push_back(parent.at(0));
				scoredAi.push_back(parent.at(1));
			}*/

			generation++;
			individual = 0;

			std::sort(scoredAi.begin(), scoredAi.end(), compareFunction);
			parent.clear();
			for (int at = 0; at < 10; at++) {
				parent.push_back(scoredAi.at(at));
			}
			scoredAi.clear();
			avgLength = lengthSum / oneGeneration;
			lengthSum = 0;

			std::ofstream output("result/" + fileName + ".res", std::ios_base::app);
			std::ofstream csv("result/" + fileName + ".csv", std::ios_base::app);
			std::string temp = "";

			for (auto pos : parent.at(0).apple) {
				temp += std::to_string(pos.x) + "," + std::to_string(pos.y) + " ";
			}
			temp += "\n";
			temp = std::to_string(parent.at(0).length) + ")" + parent.at(0).ai.get()->getWeightData() + "\n";
			output.write(temp.c_str(), temp.size());
			temp = std::to_string(parent.at(0).length) + "\n";
			csv.write(temp.c_str(), temp.size());
			output.close();
			generationBestLength = 0;

			if (generation >= runTill) {
				//runCount++;
				//oneGeneration += 10;
				if (runCount > 100) {
					exit(0);
				}

				//reset();
				std::cout << "RESETFINISH\n";
			}
		}

		if (generation == 1) {
			for (int at = 0; at < CALCULATE_ONCE; at++) {
				ai.push_back({ std::shared_ptr<Ai>(new Ai()), std::shared_ptr<Game>(new Game(BOARD_SIZE, seed)) });
				putIndividual(at);
			}
		}
		else {
			for (int at = 0; at < CALCULATE_ONCE; at++) {
				Individual temp;
				if (Tool::randomInt(0, 1) == 0) {
					temp.ai = std::shared_ptr<Ai>(new Ai(parent.at(0).ai, parent.at(1).ai));
				}
				else {
					temp.ai = std::shared_ptr<Ai>(new Ai(parent.at(1).ai, parent.at(0).ai));
				}
				temp.game = std::shared_ptr<Game>(new Game(BOARD_SIZE, seed));
				ai.push_back(temp);
				putIndividual(at);
			}
		}
		individual += CALCULATE_ONCE;
	}

	//SDL_Delay(10);
}

void renderText(SDL_Renderer *renderer) {
	Font::renderFont(renderer, "Generation : " + std::to_string(generation), 700, 0);
	Font::renderFont(renderer, "Individual : " + std::to_string(individual), 700, 20);
	if (processMode == 0) {
		Font::renderFont(renderer, "Process Mode : Visible", 700, 40);
	}
	else if (processMode == 1) {
		Font::renderFont(renderer, "Process Mode : Render All", 700, 40);
	}
	else if (processMode == 2) {
		Font::renderFont(renderer, "Process Mode : Fastest", 700, 40);
	}
	else if (processMode == 3) {
		Font::renderFont(renderer, "Process Mode : Render All Slower", 700, 40);
	}
	Font::renderFont(renderer, "Best Score : " + std::to_string(bestScore), 700, 60);
	Font::renderFont(renderer, "Avg Length : " + std::to_string(avgLength), 700, 80);
	Font::renderFont(renderer, "Best Length : " + std::to_string(bestLength), 700, 100);
}

void render(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	int num = 0;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			ai.at(num).game.get()->render(renderer, 10 + (220 + 10) * x, 10 + (220 + 10) * y, 220);
			num++;
		}
	}

	renderText(renderer);

	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_Window *window = SDL_CreateWindow("Neural Network", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 700, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	SDL_Event event;
	bool running = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(renderer, 1000, 700);

	reset();
	for (int at = 0; at < CALCULATE_ONCE; at++) {
		ai.push_back({ std::shared_ptr<Ai>(new Ai()), std::shared_ptr<Game>(new Game(BOARD_SIZE, seed)) });
		putIndividual(at);
	}
	individual += CALCULATE_ONCE;

	processMode = 2;

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		update();
		if (processMode != 2) {
			render(renderer);
		}
	}

	SDL_Quit();
	TTF_Quit();
	return 0;
}