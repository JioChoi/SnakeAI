#include "Game.h"

#define MOVE_CHANCE 150
#define APPLE_MOVE 150

Game::Game(int boardSize, int seed) : boardSize(boardSize), length(3), dead(false), apple({ 0, 0 }), win(false) {
	//srand(seed);
	mt = std::mt19937(seed);
	direction = Direction::up;
	snakeHead = { random(boardSize), random(boardSize) };
	snake.push_back(snakeHead);
	respawnApple();

	moveChance = MOVE_CHANCE;
	liveTime = 0;
	eatenApple = 0;
	previousDistance = 0;
	closePoint = 0;

	head = nullptr;
	straight = nullptr;
	corner = nullptr;
	end = nullptr;
	appleTexture = nullptr;
}

void Game::update() {
	if (!dead) {
		moveSnake();
		checkDeath();
		moveChance--;
		liveTime++;

		if (dead == true) {
			return;
		}

		double currentDistance = sqrt(pow(snakeHead.x - apple.x, 2) + pow(snakeHead.y - apple.y, 2));
		if (currentDistance < previousDistance) {
			closePoint += 1;
		}
		else {
			closePoint -= 1.5;
		}
		previousDistance = currentDistance;

		if (snakeHead.x == apple.x && snakeHead.y == apple.y) {
			respawnApple();
			length++;
			moveChance += APPLE_MOVE;
			eatenApple++;
		}
		if (moveChance < 0) {
			dead = true;
		}
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

Game::~Game() {
	SDL_DestroyTexture(head);
	SDL_DestroyTexture(straight);
	SDL_DestroyTexture(corner);
	SDL_DestroyTexture(end);
	SDL_DestroyTexture(appleTexture);
}

void Game::render(SDL_Renderer *renderer, int renderX, int renderY, int size) {
	int singleTileSize = size / boardSize;

	if (head == nullptr) {
		head = IMG_LoadTexture(renderer, "image/head.png");
		straight = IMG_LoadTexture(renderer, "image/straight.png");
		corner = IMG_LoadTexture(renderer, "image/corner.png");
		end = IMG_LoadTexture(renderer, "image/end.png");
		appleTexture = IMG_LoadTexture(renderer, "image/apple.png");
	}

	renderGrid(renderer, renderX, renderY, singleTileSize);
	renderSnake(renderer, renderX, renderY, singleTileSize);
	renderApple(renderer, renderX, renderY, singleTileSize);
}

void Game::getData(std::vector<double> &finalData) {
	finalData.clear();
	finalData.resize(24, 0);

	getData(finalData, 0, apple, snakeHead);
	rotateValue(finalData, static_cast<int>(direction) * 2, 0);

	for (int at = 0; at < snake.size() - 1; at++) {
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
}

void Game::getData(std::vector<double> &vector, int offset, SDL_Point &a, SDL_Point &b) {
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

void Game::rotateValue(std::vector<double> &vector, int amount, int offset) {
	std::vector<double> temp = vector;

	int num = amount;
	for (int at = 0; at < 8; at++) {
		if (num >= 8) {
			num = 0;
		}

		vector.at(at + offset) = temp.at(num + offset);
		num++;
	}
}

void Game::respawnApple() {
	if (length == boardSize * boardSize) {
		win = true;
		return;
	}

	bool snakeTouch = false;
	do {
		snakeTouch = false;
		apple = { random(boardSize), random(boardSize) };
		for (SDL_Point &temp : snake) {
			if (temp.x == apple.x && temp.y == apple.y) {
				snakeTouch = true;
				break;
			}
		}
	} while (snakeTouch == true);

	applePositionLog.push_back(apple);
}

void Game::renderSnake(SDL_Renderer *renderer, int renderX, int renderY, int singleTileSize) {
	std::vector<SDL_Point> tempSnake;
	for (auto &temp : snake) {
		tempSnake.push_back(temp);
	}

	for (int i = 0; i < tempSnake.size(); i++) {
		drect = { renderX + tempSnake.at(i).x * singleTileSize, renderY + tempSnake.at(i).y * singleTileSize, singleTileSize, singleTileSize };
		if (i == tempSnake.size() - 1) {
			drect = { renderX + tempSnake.at(i).x * singleTileSize - (int)(0.24 * singleTileSize), renderY + tempSnake.at(i).y * singleTileSize - (int)(0.357 * singleTileSize), (int)(singleTileSize * 1.5), (int)(singleTileSize * 1.6) };
			SDL_RenderCopyEx(renderer, head, NULL, &drect, (int)direction * 90, 0, SDL_FLIP_NONE);
		}
		else if (i == 0) {
			int dir = getDirectionBetween(tempSnake.at(0), tempSnake.at(1));
			SDL_RenderCopyEx(renderer, end, NULL, &drect, dir * 90, 0, SDL_FLIP_NONE);
		}
		else {
			int dir = getCornerData(tempSnake.at(i), tempSnake.at(i - 1), tempSnake.at(i + 1));
			if (dir == -1) {
				int a = getDirectionBetween(tempSnake.at(i), tempSnake.at(i - 1));
				int b = getDirectionBetween(tempSnake.at(i), tempSnake.at(i + 1));

				if (a == 0 && b == 2 || b == 0 && a == 2) {
					SDL_RenderCopyEx(renderer, straight, NULL, &drect, 90, 0, SDL_FLIP_NONE);
				}
				else {
					SDL_RenderCopyEx(renderer, straight, NULL, &drect, 0, 0, SDL_FLIP_NONE);
				}
			}
			else {
				SDL_RenderCopyEx(renderer, corner, NULL, &drect, (dir) * 90, 0, SDL_FLIP_NONE);
			}
		}
	}
}

int Game::getDirectionBetween(SDL_Point a, SDL_Point b) {
	int x = a.x - b.x;
	int y = a.y - b.y;

	if (x > 0) {
		return 1;
	}
	if (x < 0) {
		return 3;
	}
	if (y < 0) {
		return 0;
	}
	if (y > 0) {
		return 2;
	}
}

int Game::getCornerData(SDL_Point a, SDL_Point b, SDL_Point c) {
	int da = getDirectionBetween(a, b);
	int db = getDirectionBetween(a, c);

	if (da == 1 && db == 0) {
		return 0;
	}
	if (da == 2 && db == 1) {
		return 1;
	}
	if (da == 3 && db == 2) {
		return 2;
	}
	if (da == 0 && db == 3) {
		return 3;
	}

	if (da == 0 && db == 1) {
		return 0;
	}
	if (da == 1 && db == 2) {
		return 1;
	}
	if (da == 2 && db == 3) {
		return 2;
	}
	if (da == 3 && db == 0) {
		return 3;
	}
	return -1;
}

void Game::renderGrid(SDL_Renderer *renderer, int renderX, int renderY, int singleTileSize) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	drect = { renderX - 2, renderY - 2, singleTileSize * boardSize + 4, singleTileSize * boardSize + 4 };
	SDL_RenderFillRect(renderer, &drect);
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			if ((x + y) % 2 == 0) {
				SDL_SetRenderDrawColor(renderer, 213, 213, 213, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 201, 201, 201, 255);
			}

			drect = { renderX + x * singleTileSize, renderY + y * singleTileSize, singleTileSize, singleTileSize };
			SDL_RenderFillRect(renderer, &drect);
		}
	}
}

void Game::renderApple(SDL_Renderer *renderer, int renderX, int renderY, int singleTileSize) {
	drect = { renderX + apple.x * singleTileSize, renderY + apple.y * singleTileSize, singleTileSize, singleTileSize };
	SDL_RenderCopy(renderer, appleTexture, NULL, &drect);
}

int Game::random(int max) {
	std::uniform_int_distribution<int> dis(0, boardSize - 1);

	return dis(mt);
}

Direction Game::getDirection() {
	return direction;
}

void Game::setDirection(const Direction direction) {
	this->direction = direction;
}

void Game::moveDirection(int change) {
	int dir = static_cast<int>(direction);
	dir += change;

	if (dir < 0) {
		dir = 3;
	}
	else if (dir > 3) {
		dir = 0;
	}
	direction = static_cast<Direction>(dir);
}
