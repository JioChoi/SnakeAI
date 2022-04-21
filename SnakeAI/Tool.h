#pragma once
#include <iostream>
#include <random>
#include <SDL.h>

class RandomEngine {
public:
	RandomEngine();
	int randomInt(int min, int max);
	double randomDouble(double min, double max);
private:
	std::mt19937 engine;
};

class Tool {
public:
	Tool();
	static void resetSeed();
	static void updateKeyboardState();
	static bool getKeyboardState(int scancode);
	static int randomInt(int min, int max);
	static double randomDouble(double min, double max);
private:
	static const Uint8 *keyboardState;
	static RandomEngine *engine;
};