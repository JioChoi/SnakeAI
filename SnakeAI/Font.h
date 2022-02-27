#pragma once
#include <iostream>
#include <string>
#include <SDL_ttf.h>

class Font {
public:
	static void renderFont(SDL_Renderer* renderer, std::string text, int x, int y);
private:
	static TTF_Font* font;
};