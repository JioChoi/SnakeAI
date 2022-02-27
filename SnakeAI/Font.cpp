#include "Font.h"

TTF_Font* Font::font = nullptr;

void Font::renderFont(SDL_Renderer* renderer, std::string text, int x, int y) {
	if (font == nullptr) {
		font = TTF_OpenFont("NotoLight.ttf", 14);
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255});
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect drect;

	SDL_QueryTexture(texture, NULL, NULL, &drect.w, &drect.h);
	drect.x = x;
	drect.y = y;

	SDL_RenderCopy(renderer, texture, NULL, &drect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}