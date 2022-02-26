#pragma once
#include <iostream>
#include <SDL.h>

class Tool {
public:
	static void updateKeyboardState();
	static bool getKeyboardState(int scancode);
private:
	static const Uint8 *keyboardState;
};