#include "Tool.h"

const Uint8 *Tool::keyboardState;
RandomEngine *Tool::engine = nullptr;

double RandomEngine::randomDouble(double min, double max) {
	std::uniform_real_distribution<float> dis(min, max);
	return dis(engine);
}

int RandomEngine::randomInt(int min, int max) {
	std::uniform_int_distribution<int> dis(min, max);
	return dis(engine);
}

RandomEngine::RandomEngine() {
	engine = std::mt19937(191917);
	//std::random_device rd;
	//engine = std::mt19937(rd());
}

Tool::Tool() {
	resetSeed();
}

void Tool::resetSeed() {
	if (engine != nullptr) {
		delete engine;
	}
	engine = new RandomEngine();
}

void Tool::updateKeyboardState() {
	keyboardState = SDL_GetKeyboardState(NULL);
}

bool Tool::getKeyboardState(int scancode) {
	return keyboardState[scancode];
}

int Tool::randomInt(int min, int max) {
	return engine->randomInt(min, max);
}

double Tool::randomDouble(double min, double max) {
	return engine->randomDouble(min, max);
}

