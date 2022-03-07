#pragma once
#include <iostream>
#include <random>
#include <vector>

#include <SDL.h>

enum class Direction {
	up = 0,
	right = 1,
	down = 2,
	left = 3
};

class Game {
public:
	Game(int boardSize, int seed);
	void update();
	void render(SDL_Renderer *renderer, int renderX, int renderY, int size);

	void getData(std::vector<float> &finalData);

	Direction getDirection();
	void setDirection(const Direction direction);
	void moveDirection(int change);
	bool win;
	bool dead;

	int moveChance;
	int liveTime;
	int eatenApple;
	int length;
	float closePoint;

	std::vector<SDL_Point> applePositionLog;
private:
	void moveSnake();
	void checkDeath();
	int random(int max);

	void renderSnake(SDL_Renderer *renderer, int renderX, int renderY, int singleTileSize);
	void renderGrid(SDL_Renderer *renderer, int renderX, int renderY, int singleTileSize);
	void renderApple(SDL_Renderer *renderer, int renderX, int renderY, int singleTileSize);

	void getData(std::vector<float>& vector, int offset, SDL_Point &a, SDL_Point &b);
	void rotateValue(std::vector<float>& vector, int amount, int offset);

	void respawnApple();

	SDL_Point snakeHead;
	std::vector<SDL_Point> snake;
	Direction direction;

	SDL_Point apple;

	int boardSize;
	SDL_Rect drect;
	std::mt19937 mt;

	float previousDistance;
};