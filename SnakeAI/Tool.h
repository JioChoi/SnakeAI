#pragma once
#include <iostream>
#include <random>
#include <SDL.h>

class Tool {
public:
	Tool();
	static void updateKeyboardState();
	static bool getKeyboardState(int scancode);
	static int randomInt(int min, int max);
	static float randomFloat(float min, float max);
private:
	static const Uint8 *keyboardState;
	static std::mt19937 mt;
};