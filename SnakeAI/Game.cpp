#include "Game.h"

Game::Game(int boardSize, int seed) : boardSize(boardSize), length(3), dead(false) {
	srand(seed);
	direction = Direction::up;
	snakeHead = { random(boardSize), random(boardSize) };
	apple = { random(boardSize), random(boardSize) };
	snake.push_back(snakeHead);
}

void Game::update() {
	if (!dead) {
		moveSnake();
		checkDeath();
	}
}

void Game::checkDeath() {
	if (snakeHead.x < 0 || snakeHead.x >= boardSize || snakeHead.y < 0 || snakeHead.y >= boardSize) {
		dead = true;
	}

	for (int at = 0; at < snake.size() - 1; at++) {
		if (snakeHead.x == snake.at(at).x && snakeHead.y == snake.at(at).y) {
			dead = true;
			break;
		}
	}
}

void Game::moveSnake() {
	switch (direction) {
	case Direction::up:
		snakeHead.y--;
		break;
	case Direction::down:
		snakeHead.y++;
		break;
	case Direction::right:
		snakeHead.x++;
		break;
	case Direction::left:
		snakeHead.x--;
		break;
	}
	
	snake.push_back(snakeHead);
	if (snake.size() > length) {
		snake.erase(snake.begin());
	}
}

void Game::render(SDL_Renderer *renderer, int renderX, int renderY, int size) {
	int singleTileSize = size / boardSize;

	renderSnake(renderer, renderX, renderY, singleTileSize);
	renderApple(renderer, renderX, renderY, singleTileSize);
	renderGrid(renderer, renderX, renderY, singleTileSize);
}

std::vector<float> Game::getData() {
	std::vector<float> finalData;
	finalData.resize(24, 0);

	getData(finalData, 0, apple, snakeHead);
	rotateValue(finalData, static_cast<int>(direction) * 2, 0);

	for(int at = 0; at < snake.size() - 1; at++) {
		getData(finalData, 8, snake.at(at), snakeHead);
	}
	rotateValue(finalData, static_cast<int>(direction) * 2, 8);

	finalData.at(16 + 0) = (snakeHead.y + 1);
	finalData.at(16 + 2) = (boardSize - snakeHead.x);
	finalData.at(16 + 4) = (boardSize - snakeHead.y);
	finalData.at(16 + 6) = (snakeHead.x + 1);
	finalData.at(16 + 1) = std::max(finalData.at(16 + 0), finalData.at(16 + 2));
	finalData.at(16 + 3) = std::max(finalData.at(16 + 4), finalData.at(16 + 2));
	finalData.at(16 + 5) = std::max(finalData.at(16 + 4), finalData.at(16 + 6));
	finalData.at(16 + 7) = std::max(finalData.at(16 + 0), finalData.at(16 + 6));
	rotateValue(finalData, static_cast<int>(direction) * 2, 16);

	return finalData;
}

void Game::getData(std::vector<float>& vector, int offset, SDL_Point& a, SDL_Point& b) {
	int xdiff = a.x - b.x;
	int ydiff = a.y - b.y;

	if (a.x == b.x && ydiff < 0) {
		vector.at(offset + 0) = -ydiff;
	}
	else if (a.y == b.y && xdiff > 0) {
		vector.at(offset + 2) = xdiff;
	}
	else if (a.x == b.x && ydiff > 0) {
		vector.at(offset + 4) = ydiff;
	}
	else if (a.y == b.y && xdiff < 0) {
		vector.at(offset + 6) = -xdiff;
	}
	else if (abs(xdiff) == abs(ydiff)) {
		if (xdiff > 0 && ydiff < 0) {
			vector.at(offset + 1) = xdiff;
		}
		else if (xdiff > 0 && ydiff > 0) {
			vector.at(offset + 3) = xdiff;
		}
		else if (xdiff < 0 && ydiff > 0) {
			vector.at(offset + 5) = -xdiff;
		}
		else if (xdiff < 0 && ydiff < 0) {
			vector.at(offset + 7) = -xdiff;
		}
	}
}

void Game::rotateValue(std::vector<float>& vector, int amount, int offset) {
	std::vector<float> temp = vector;

	int num = amount;
	for (int at = 0; at < 8; at++) {
		if (num >= 8) {
			num = 0;
		}

		vector.at(at + offset) = temp.at(num + offset);
		num++;
	}
}

void Game::renderSnake(SDL_Renderer *renderer, int renderX, int renderY, int singleTileSize) {
	if (dead) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	}
	for (auto temp : snake) {
		drect = { renderX + temp.x * singleTileSize, renderY + temp.y * singleTileSize, singleTileSize, singleTileSize };
		SDL_RenderFillRect(renderer, &drect);
	}
}

void Game::renderGrid(SDL_Renderer *renderer, int renderX, int renderY, int singleTileSize) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			drect = { renderX + x * singleTileSize, renderY + y * singleTileSize, singleTileSize, singleTileSize };
			SDL_RenderDrawRect(renderer, &drect);
		}
	}
}

void Game::renderApple(SDL_Renderer* renderer, int renderX, int renderY, int singleTileSize) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	drect = { renderX + apple.x * singleTileSize, renderY + apple.y * singleTileSize, singleTileSize, singleTileSize };
	SDL_RenderFillRect(renderer, &drect);
}

int Game::random(int max) {
	return rand() % (max + 1);
}

Direction Game::getDirection() {
	return direction;
}

void Game::setDirection(const Direction direction) {
	this->direction = direction;
}