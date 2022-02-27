#include "Tool.h"

const Uint8 *Tool::keyboardState;
std::mt19937 Tool::mt;

Tool::Tool() {
	std::random_device rd;
	mt = std::mt19937(rd());
}

void Tool::updateKeyboardState() {
	keyboardState = SDL_GetKeyboardState(NULL);
}

bool Tool::getKeyboardState(int scancode) {
	return keyboardState[scancode];
}

int Tool::randomInt(int min, int max) {
	std::uniform_int_distribution<int> dis(min, max);
	return dis(mt);
}

float Tool::randomFloat(float min, float max) {
	std::uniform_real_distribution<float> dis(min, max);
	return dis(mt);
}

