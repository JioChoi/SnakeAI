#include "Tool.h"

const Uint8 *Tool::keyboardState;

void Tool::updateKeyboardState() {
	keyboardState = SDL_GetKeyboardState(NULL);
}

bool Tool::getKeyboardState(int scancode) {
	return keyboardState[scancode];
}